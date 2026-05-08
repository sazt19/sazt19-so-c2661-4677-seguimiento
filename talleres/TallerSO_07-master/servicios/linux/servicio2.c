#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_SENALES 5

static volatile int contador_senales = 0;
static char syslog_buffer[BUFFER_SIZE];

void manejador_senal(int signo);

int main(int argc, char const *argv[]) {
    struct sigaction nuevo_sa, viejo_sa;
    nuevo_sa.sa_handler = manejador_senal;
    sigemptyset(&nuevo_sa.sa_mask);
    nuevo_sa.sa_flags = 0;

    sigaction(SIGTERM, &nuevo_sa, &viejo_sa);
    sigaction(SIGINT,  &nuevo_sa, &viejo_sa);
    sigaction(SIGTSTP, &nuevo_sa, &viejo_sa);

    openlog("servicio2", LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_INFO | LOG_USER, "%s", "Servicio2 iniciado");

    int paso = 0;
    for (;;) {
        snprintf(syslog_buffer, BUFFER_SIZE,
                 "Ejecutando paso %d (senales recibidas: %d/%d)",
                 paso++, contador_senales, MAX_SENALES);
        syslog(LOG_INFO | LOG_USER, "%s", syslog_buffer);
        sleep(10);
    }

    return EXIT_SUCCESS;
}

void manejador_senal(int signo) {
    contador_senales++;

    const char *nombre;
    switch (signo) {
        case SIGTERM:  nombre = "SIGTERM";  break;
        case SIGINT:   nombre = "SIGINT";   break;
        case SIGTSTP:  nombre = "SIGTSTP";  break;
        default:       nombre = "DESCONOCIDA"; break;
    }

    snprintf(syslog_buffer, BUFFER_SIZE,
             "Senal recibida: %s (%d). Contador: %d/%d",
             nombre, signo, contador_senales, MAX_SENALES);
    syslog(LOG_INFO | LOG_USER, "%s", syslog_buffer);

    if (contador_senales >= MAX_SENALES) {
        syslog(LOG_INFO | LOG_USER, "%s", "Contador llego a 5. Terminando.");
        closelog();
        _exit(EXIT_SUCCESS);
    }
}
