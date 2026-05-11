#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/*  Estructura de control lateral                                       */
struct ControlLateral {
    sem_t activacion;       /* proyectil → hilo: "pase por tu etapa, dispara" */
    sem_t accion_disparo;   /* hilo → proyectil: "dispare la carga lateral"   */
    sem_t recarga;          /* recarga → hilo: "ya puedes esperar otro disparo"*/
    struct ControlLateral *sig_etapa;
    pthread_t tid_hilo_control_lateral;
    int numero_etapa;       /* solo para trazas de depuracion */
};

/*  Hilo de control de disparo lateral                                  */
/*
 * Ciclo de vida del hilo de cada etapa:
 *   1. Espera que el proyectil active la etapa  (wait activacion)
 *   2. Dispara la carga lateral  (simulado con printf)
 *   3. Señala al proyectil que la carga fue efectuada (post accion_disparo)
 *   4. Espera ser recargado para el siguiente disparo (wait recarga)
 *   5. Vuelve a 1
 *
 * El hilo termina cuando recibe -1 en sem_activacion (convenio de
 * "envenenamiento": el disparador manda sem_post justo antes de unirse,
 * y el hilo verifica si sig_etapa == NULL como señal de fin, pero lo
 * más limpio es simplemente hacer el ciclo tantas veces como disparos
 * se vayan a realizar.  Aquí usamos un bucle infinito y dejamos que
 * pthread_cancel / pthread_join lo termine desde fuera, o bien
 * controlamos el número de disparos desde main.
 */
static void *hilo_control_disparo_lateral(void *arg) {
    struct ControlLateral *pControlLateral = (struct ControlLateral *) arg;

    /* El hilo vive mientras haya disparos por realizar.
     * En esta implementación se usa un bucle infinito; el hilo es
     * cancelado (pthread_cancel) por el destructor o simplemente se
     * deja terminar al salir del proceso. */
    while (1) {
        /* 1. Esperar activación por parte del proyectil */
        sem_wait(&pControlLateral->activacion);

        /* 2. Disparar carga lateral */
        printf("  [Etapa %d] *** DISPARO LATERAL ***\n",
               pControlLateral->numero_etapa);

        /* 3. Señalar al proyectil que la carga lateral fue efectuada */
        sem_post(&pControlLateral->accion_disparo);

        /* 4. Esperar recarga antes de estar listo para el siguiente disparo */
        sem_wait(&pControlLateral->recarga);

        printf("  [Etapa %d] Recargada y lista.\n",
               pControlLateral->numero_etapa);
    }

    return NULL;
}

/*  iniciarV3                                                           */
/*
 * Construye el arreglo de estructuras de control lateral completamente
 * conectado (lista enlazada circular / lineal según necesidad).
 * Lanza el hilo de cada etapa.
 * Devuelve el puntero al arreglo (también sirve como puntero a la
 * primera etapa).
 */
struct ControlLateral *iniciarV3(int n_etapas) {
    struct ControlLateral *estructura_control =
        malloc(sizeof(struct ControlLateral) * n_etapas);

    if (estructura_control == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_etapas; i++) {
        /* Inicializar semáforos en 0 (bloqueados) */
        sem_init(&estructura_control[i].activacion,    0, 0);
        sem_init(&estructura_control[i].accion_disparo, 0, 0);
        sem_init(&estructura_control[i].recarga,        0, 0);

        estructura_control[i].numero_etapa = i + 1;

        /* Conectar con la siguiente etapa (la última apunta a NULL) */
        if (i < n_etapas - 1) {
            estructura_control[i].sig_etapa = &estructura_control[i + 1];
        } else {
            estructura_control[i].sig_etapa = NULL;
        }

        /* Lanzar hilo de control de disparo lateral para esta etapa */
        pthread_create(
            &estructura_control[i].tid_hilo_control_lateral,
            NULL,
            hilo_control_disparo_lateral,
            &estructura_control[i]
        );
    }

    return estructura_control;
}
/*  control_proyectil                                                   */
/*
 * Controla el paso del proyectil por todas las etapas:
 *   Para cada etapa i:
 *     1. Señala a la etapa i que el proyectil llegó (post activacion[i])
 *     2. Espera a que la carga lateral haya sido disparada (wait accion_disparo[i])
 *     3. Continúa a la siguiente etapa
 */
void control_proyectil(struct ControlLateral estructura_control[],
                       int n_etapas) {
    printf("[Proyectil] Iniciando recorrido por %d etapas...\n", n_etapas);

    for (int i = 0; i < n_etapas; i++) {
        printf("[Proyectil] Entrando a etapa %d\n", i + 1);

        /* 1. Activar el disparo lateral de esta etapa */
        sem_post(&estructura_control[i].activacion);

        /* 2. Esperar confirmación del disparo lateral */
        sem_wait(&estructura_control[i].accion_disparo);

        printf("[Proyectil] Etapa %d completada. Mas velocidad\n", i + 1);
    }

    printf("[Proyectil] Recorrido completo Velocidad maxima alcanzada.\n\n");
}

/*  control_recarga                                                     */
/*
 * Recarga cada una de las etapas para preparar el siguiente disparo:
 *   Para cada etapa i:
 *     Señala que la etapa i está recargada (post recarga[i])
 */
void control_recarga(struct ControlLateral estructura_control[],
                     int n_etapas) {
    printf("[Recarga]   Recargando todas las etapas...\n");

    for (int i = 0; i < n_etapas; i++) {
        sem_post(&estructura_control[i].recarga);
    }

    printf("[Recarga]   Todas las etapas recargadas.\n\n");
}

int main(void) {
    const int N_ETAPAS   = 5;
    const int N_DISPAROS = 3;

    printf("=== V-3 Control System  (%d etapas, %d disparos) ===\n\n",
           N_ETAPAS, N_DISPAROS);

    struct ControlLateral *v3 = iniciarV3(N_ETAPAS);

    for (int disparo = 1; disparo <= N_DISPAROS; disparo++) {
        printf("--- Disparo %d ---\n", disparo);
        control_proyectil(v3, N_ETAPAS);

        if (disparo < N_DISPAROS) {
            control_recarga(v3, N_ETAPAS);
        }
    }

    /* Liberar recursos */
    for (int i = 0; i < N_ETAPAS; i++) {
        pthread_cancel(v3[i].tid_hilo_control_lateral);
        pthread_join(v3[i].tid_hilo_control_lateral, NULL);
        sem_destroy(&v3[i].activacion);
        sem_destroy(&v3[i].accion_disparo);
        sem_destroy(&v3[i].recarga);
    }
    free(v3);

    printf("=== Fin ===\n");
    return 0;
}

