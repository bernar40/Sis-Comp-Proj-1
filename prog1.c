#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "tratador.h"
#define CHAR sizeof(char)
int decodifica_params(int **vet,char *params);



int main(int argc,char *argv[]){
	char *params = argv[1];
	int *raj, tam;
	printf("%s\n", params);
	int i, j;
	int my_pid = getpid();

	raj = separador_tempo(params, &tam);

	raise(SIGSTOP);
	for(i=0;i<tam;i++){
		for(j=0;j<raj[i];j++){
			printf("%d\n",my_pid);	//Output espacíficado pelo enunciado
			sleep(1);
		}
		//condição I/O
		kill(getppid(),SIGUSR1);
		sleep(3);
		raise(SIGSTOP);
	}
	//Acabou execução
	kill(getppid(),SIGUSR2);
	exit(0);
}