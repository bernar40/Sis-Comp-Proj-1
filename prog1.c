#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tratador.h"
#define CHAR sizeof(char)
int decodifica_params(int **vet,char *params);

//void tratador_customSTOP(int signal);
//void tratador_customCONT(int signal);

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
