#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tratador.h"

int main (int argc, char *argv[]){
	int tam, *exec_time = separador_tempo(argv[1], &tam);
	int i, j;
	//printf("\nExec_time = (%d, %d, %d)",exec_time[0],exec_time[1],exec_time[2]);
	raise(SIGSTOP);
	for (i=0; i<tam; i++){
		//printf("\n\tPID: %d ---- Exec time: %d", getpid(), exec_time[i]);
		for (j=0; j<exec_time[i]; j++){
			printf("\n%d", getpid());
			sleep(1);
		}
		//condição I/O
		if(i<(tam-1)){
			printf("\n%d\tentrou em IO",getpid());
			kill(getppid(),SIGUSR1);
			sleep(3);
			kill(getppid(),SIGCHLD);
			//raise(SIGSTOP);
		}
	}
	printf("\n\tEu, %d, Morri\n",getpid());
	kill(getppid(),SIGUSR2);
	return 0;
}
