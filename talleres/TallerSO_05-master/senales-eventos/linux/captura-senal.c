#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "manejadores.h"

const unsigned int SLEEP_TIME = 4;
extern int contador;

int
main(int argc, char *argv[]) {
  struct sigaction viejo_sa;
  struct sigaction nuevo_sa;
  
  nuevo_sa.sa_handler = manejador_sigint;
  sigemptyset(&nuevo_sa.sa_mask);
  nuevo_sa.sa_flags = 0;
  sigaction(SIGINT, &nuevo_sa, &viejo_sa);

  pid_t pId = getpid();
  
  while (contador < 4) {
    fprintf(stdout,
	    "Proceso: %d Esperando por %d\n",
	    pId, SIGINT);
    sleep(SLEEP_TIME);
  }

  sigaction(SIGINT, &viejo_sa, NULL);

  fprintf(stdout, "Terminado despues de: %d\n", contador);
  return 0;
}
