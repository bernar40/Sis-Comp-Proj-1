#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tratador.h"
#define CHAR sizeof(char)
int decodifica_params(int **vet,char *params);



int main(int argc,char *argv[]){
	char *params = argv[1];
	int *raj, tam;
	int i, j;
	int my_pid = getpid();

	raj = separador_tempo(params, &tam);
	
	raise(SIGSTOP);
	for(i=0;i<tam;i++){
		printf("\nDe %d: rajada %d", getpid(),raj[i]);
		for(j=0;j<raj[i];j++){
			printf("\n%d",my_pid);	//Output espacíficado pelo enunciado
			sleep(1);
		}
		//condição I/O
		kill(getppid(),SIGUSR1);
		printf("\nCriança %d dormiu",getpid());
		sleep(3);
		printf("\nCriança %d acordou",getpid());
		kill(getppid(),SIGCHLD);
		printf("\nCriança %d avisou que acordou",getpid());
		raise(SIGSTOP);
	}
	//Acabou execução
	printf("\nCriança %d avisa que morreu",proc->my_pid);
	kill(getppid(),SIGUSR2);
	exit(0);
}
