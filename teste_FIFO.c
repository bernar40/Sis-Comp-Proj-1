#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#define FIFO1 "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo1"
#define FIFO2 "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo2"


int main (void){
	int fpFIFO1, fpFIFO2;
	int ch;
	int *exect;
	int i;

	if (access(FIFO1, F_OK) == -1){
		if(mkfifo(FIFO1, S_IRUSR | S_IWUSR) != 0){ //Cria o fifo com as devidas permissoes
			puts("Erro ao criar FIFO1");
			return -1;
		}
	}
	if (access(FIFO2, F_OK) == -1){
		if(mkfifo(FIFO2, S_IRUSR | S_IWUSR) != 0){ //Cria o segundo fifo, um sera para o tamanho e outro para o vetor
			puts("Erro ao criar FIFO2");
			return -1;
		}
	}

	if ((fpFIFO1 = open(FIFO1, O_RDONLY)) < 0){ //Abre o fifo para leitura e bloqueia, ele espera os dados chegarem, acho que mudaremos isso para sem bloquei
		puts("Erro ao abrir FIFO1");
		return -2;
	}
	read(fpFIFO1, &ch, sizeof(ch));
	printf("%d\n", ch);
	if ((fpFIFO2 = open(FIFO2, O_RDONLY)) < 0){ //Abre o fifo para leitura e bloqueia, ele espera os dados chegarem, acho que mudaremos isso para sem bloquei
		puts("Erro ao abrir FIFO2");
		return -2;
	}
	exect = (int *)malloc((ch*sizeof(int)));
	for (i=0; i<ch; i++){
		read(fpFIFO2, &exect[i], sizeof(exect[i]));
		printf("%d\n",exect[i]);
	}
	
	
	
	
		
	close(fpFIFO1);
	close(fpFIFO2);
	
	return 0;
}