#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#define BUFFER_SIZE  1024
#define FIFO_ENTRADA "/tmp/eco_entrada"
#define FIFO_SALIDA  "/tmp/eco_salida"
#define LOG_FILE     "/tmp/servicio1.log"

static volatile int running = 1;

void log_msg(const char *msg) {
    openlog("servicio1", LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_INFO | LOG_USER, "%s", msg);
    closelog();
    FILE *f = fopen(LOG_FILE, "a");
    if (f) { fprintf(f, "%s\n", msg); fclose(f); }
}

void manejador_sigterm(int signo);
void invertir_caso(char *str);

int main(int argc, char const *argv[]) {
    struct sigaction nuevo_sa, viejo_sa;
    nuevo_sa.sa_handler = manejador_sigterm;
    sigemptyset(&nuevo_sa.sa_mask);
    nuevo_sa.sa_flags = 0;
    sigaction(SIGTERM, &nuevo_sa, &viejo_sa);
    sigaction(SIGINT,  &nuevo_sa, &viejo_sa);

    log_msg("Iniciando servicio1");

    if (mkfifo(FIFO_ENTRADA, 0666) == -1 && errno != EEXIST) {
        log_msg("Error creando FIFO entrada");
        _exit(EXIT_FAILURE);
    }
    if (mkfifo(FIFO_SALIDA, 0666) == -1 && errno != EEXIST) {
        log_msg("Error creando FIFO salida");
        unlink(FIFO_ENTRADA);
        _exit(EXIT_FAILURE);
    }

    log_msg("FIFOs creadas: /tmp/eco_entrada, /tmp/eco_salida");

    while (running) {
        int fd_entrada = open(FIFO_ENTRADA, O_RDONLY);
        if (fd_entrada == -1) {
            if (errno == EINTR) continue;
            log_msg("Error abriendo FIFO entrada");
            break;
        }

        int fd_salida = open(FIFO_SALIDA, O_WRONLY);
        if (fd_salida == -1) {
            log_msg("Error abriendo FIFO salida");
            close(fd_entrada);
            break;
        }

        char buf[BUFFER_SIZE];
        ssize_t n;
        while ((n = read(fd_entrada, buf, sizeof(buf) - 1)) > 0) {
            buf[n] = '\0';

            char log_recibido[BUFFER_SIZE];
            strncpy(log_recibido, buf, BUFFER_SIZE - 1);
            log_recibido[BUFFER_SIZE - 1] = '\0';
            if (n > 0 && log_recibido[n-1] == '\n') log_recibido[n-1] = '\0';

            invertir_caso(buf);

            char log_enviado[BUFFER_SIZE];
            strncpy(log_enviado, buf, BUFFER_SIZE - 1);
            log_enviado[BUFFER_SIZE - 1] = '\0';
            if (strlen(log_enviado) > 0 && log_enviado[strlen(log_enviado)-1] == '\n')
                log_enviado[strlen(log_enviado)-1] = '\0';

            char log_linea[BUFFER_SIZE];
            snprintf(log_linea, BUFFER_SIZE,
                     "Recibido: '%s' -> Enviado: '%s'",
                     log_recibido, log_enviado);
            log_msg(log_linea);

            write(fd_salida, buf, n);
        }

        close(fd_entrada);
        close(fd_salida);
    }

    unlink(FIFO_ENTRADA);
    unlink(FIFO_SALIDA);
    log_msg("Servicio1 terminado");
    return EXIT_SUCCESS;
}

void invertir_caso(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isupper((unsigned char)str[i]))
            str[i] = tolower((unsigned char)str[i]);
        else if (islower((unsigned char)str[i]))
            str[i] = toupper((unsigned char)str[i]);
    }
}

void manejador_sigterm(int signo) {
    char msg[64];
    snprintf(msg, sizeof(msg), "Recibida senal %d, terminando", signo);
    log_msg(msg);
    running = 0;
    int fd = open(FIFO_ENTRADA, O_WRONLY | O_NONBLOCK);
    if (fd != -1) close(fd);
}
