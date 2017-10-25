#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include "fifo.h"


int main (void){
		char *argv[3];1];
		int my_pid = getpid();
		int fpFIFO_nome, fpFIFO_tam, fpFIFO_tempos;
		int tam;
		int *exect;
		char name[MAX_BUF];
		char params[MAX_BUF];
		//LOOP do FILHO
		/*fd = open(myfifo, O_RDONLY);
		read(fd, buf, 1024);
		printf("Received: %s \n", buf);
		close(fd);*/
		
	    fpFIFO_nome = abre_fifo_read(fpFIFO_nome, FIFO_nome); //abre FIFO
		read(fpFIFO_nome, name, MAX_BUF); //le o nome do programa e o poe no vetor name
		printf("Nome do programa: %s\n", name);

		fpFIFO_tam = abre_fifo_read(fpFIFO_tam, FIFO_tam);
		read(fpFIFO_tam, &tam, sizeof(int)); //le o tam do vetor exect no interpretador pra criar um igual aqui e o poe na var tam name
		printf("Tamanho: %d\n", tam);

		fpFIFO_tempos = abre_fifo_read(fpFIFO_tempos, FIFO_tempos);
		exect = (int *)malloc((tam*sizeof(int)));
		/*for (int i=0; i<tam; i++){
			read(fpFIFO_tempos, &exect[1], sizeof(int)); //le cada indice do vetor exect no interpretador e os poe no vetor exect
			printf("%d\n",exect[1]);
		}*/
		
		//read adaptado
		read(fpFIFO_tempos, params, MAX_BUF);
		printf("Tempos: %s\n", params);
		
		//fecha os FIFOs
		close(fpFIFO_nome); 
		close(fpFIFO_tam);
		close(fpFIFO_tempos);

		argv[0] = name;
		argv[1] = params;
		argv[2] = NULL;
		envp[0] = 0;

		execv(name, argv);
	
	return 0;

}