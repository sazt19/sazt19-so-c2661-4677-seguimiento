#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void esperar_senal(void);
void verifica_estado(int estado);

int
main(int argc, char* const argv[], char* const env[]) {
  pid_t pid_hijo_no_maneja_senal = fork();

  if (pid_hijo_no_maneja_senal == 0) {
    esperar_senal();
    _exit(EXIT_FAILURE);
  }

  
  kill(pid_hijo_no_maneja_senal, SIGALRM);

  int estado;
  if (waitpid(pid_hijo_no_maneja_senal, &estado, 0) == pid_hijo_no_maneja_senal) {
    verifica_estado(estado);
  }

  pid_t pid_hijo_ignora_senal = fork();

  if (pid_hijo_ignora_senal == 0) {
    esperar_senal();
    _exit(EXIT_FAILURE);
  }

  kill(pid_hijo_ignora_senal, SIGCHLD);

  int tiempo_espera = 5;
  alarm(tiempo_espera);

  pid_t hijo_esperado = waitpid(pid_hijo_ignora_senal, &estado, 0);
  if (hijo_esperado == pid_hijo_ignora_senal) {
    verifica_estado(estado);
  }

  return EXIT_SUCCESS;  
}

void esperar_senal(void) {
  pause();
}

void verifica_estado(int estado) { 
    if (WIFEXITED(estado)) {
      fprintf(stdout, "Proceso termino invocando _exit: %d\n",
	      WEXITSTATUS(estado));
    }
    else if (WIFSIGNALED(estado)) {
      fprintf(stdout, "Proceso senalizado por: %d\n",
	      WTERMSIG(estado));
    }
}
