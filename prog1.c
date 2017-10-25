#include <stdio.h>
#define CHAR sizeof(char)
int decodifica_params(int **vet,char *params);

int main(int argc,char *argv[]){
	char *params = argv[0];
	int *raj, tam=decodifica_params(&raj,params);
	raise(SIGSTOP);
	for(int i=0;i<tam;i++){
		for(int j=0;j<raj[i];j++){
			printf("\n%d",my_pid);	//Output espacíficado pelo enunciado
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
int decodifica_params(int **vet,char *params){
	int tam=1;
	int temp=0;
	char *init, *end;
	if(params[0]!="("){
		printf("\nprog1 recebeu parâmetro: não comecou com \"(\"");
		exit(0);
	}
	init = params+CHAR;
	for(int i=0;params[i]!=")";i++)
		if(params[i]==",")
			tam++;
	*vet = (int*)malloc(sizeof(int)*tam);
	do{
		*vet[temp] = strtol(init,&end,10);
		init = end;
		temp++;
	}while(*init!=")");
	return tam;
}
