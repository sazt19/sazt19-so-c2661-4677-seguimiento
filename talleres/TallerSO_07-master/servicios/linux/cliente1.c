#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define FIFO_ENTRADA "/tmp/eco_entrada"
#define FIFO_SALIDA  "/tmp/eco_salida"

int main(int argc, char const *argv[]) {
    char linea[BUFFER_SIZE];

    printf("Cliente eco (Ctrl+D para salir)\n");
    printf("Escribe un mensaje: ");
    fflush(stdout);

    while (fgets(linea, sizeof(linea), stdin) != NULL) {
        size_t len = strlen(linea);
        if (len == 0) {
            printf("Escribe un mensaje: ");
            fflush(stdout);
            continue;
        }

        int fd_entrada = open(FIFO_ENTRADA, O_WRONLY);
        if (fd_entrada == -1) {
            fprintf(stderr, "Error: No se pudo abrir FIFO entrada (%d). "
                    "¿Está corriendo servicio1?\n", errno);
            return EXIT_FAILURE;
        }

        int fd_salida = open(FIFO_SALIDA, O_RDONLY);
        if (fd_salida == -1) {
            fprintf(stderr, "Error: No se pudo abrir FIFO salida (%d)\n", errno);
            close(fd_entrada);
            return EXIT_FAILURE;
        }

        if (write(fd_entrada, linea, len) == -1) {
            fprintf(stderr, "Error escribiendo en FIFO entrada: %d\n", errno);
            close(fd_entrada);
            close(fd_salida);
            return EXIT_FAILURE;
        }
        close(fd_entrada);

        char respuesta[BUFFER_SIZE];
        ssize_t n = read(fd_salida, respuesta, sizeof(respuesta) - 1);
        close(fd_salida);

        if (n > 0) {
            respuesta[n] = '\0';
            printf("Respuesta: %s", respuesta);
        } else {
            fprintf(stderr, "Error leyendo respuesta\n");
        }

        printf("Escribe un mensaje: ");
        fflush(stdout);
    }

    printf("\nCliente terminado.\n");
    return EXIT_SUCCESS;
}
