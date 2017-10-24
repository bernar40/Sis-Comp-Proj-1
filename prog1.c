#include <stdio.h>

int main(int argc,char *argv[]){
	char *params = argv[0];
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
