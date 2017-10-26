#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tratador.h"

int main (int argc, char *argv[]){
	int tam, *exec_time = separador_tempo(argv[1], &tam);
	int i, j;
	for (i=0; i<tam; i++){
		printf("PID: %d ---- Exec time: %d\n", getpid(), exec_time[i]);
		for (j=0; j<exec_time[i]; j++){
			printf("%d\n", getpid());
			//sleep(1);
		}
	}
	return 0;
}
/*

int main(int argc,char *argv[]){
	char *params = argv[1];
	int *raj, tam;
	int i, j;
	int my_pid = getpid();
	printf("PID prog1: %d\n", my_pid);
	printf("Prog1 Parent PID: %d\n", getppid());
	raj = separador_tempo(params, &tam);

	
	//raise(SIGSTOP);
	for(i=0;i<tam;i++){
		printf("De %d: rajada %d\n", getpid(),raj[i]);
		for(j=0;j<raj[i];j++){
			printf("%d\n",my_pid);	//Output espacíficado pelo enunciado
			sleep(1);
		}
		//condição I/O
		kill(getppid(),SIGUSR1);
		sleep(3);
		kill(getppid(),SIGCHLD);
		//raise(SIGSTOP);
	}
	//Acabou execução
	kill(getppid(),SIGUSR2);
	return 0;
}
*/