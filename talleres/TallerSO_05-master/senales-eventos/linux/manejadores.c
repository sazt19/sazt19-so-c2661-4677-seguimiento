#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "manejadores.h"

int contador = 0;

void
manejador_sigint(int signo) {
  
  // signal(SIGINT, manejador_sigint);
  struct sigaction actual_sa;

  sigemptyset(&actual_sa.sa_mask);
  actual_sa.sa_flags = 0;
  actual_sa.sa_handler = manejador_sigint;
  ++contador;
  sigaction(SIGINT, &actual_sa, NULL);
  fprintf(stdout, "Obtuvo: %d despues: %d\n", signo, contador);
}
