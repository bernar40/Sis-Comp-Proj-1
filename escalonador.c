//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include "escalonador.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include "fila.h"
#include "fifo.h"
#define NIVEL1 1
#define NIVEL2 2
#define NIVEL3 3
#define DELTA1 1
#define DELTA2 2
#define DELTA3 4
#define ESPERA 10	//Quanto o escalonador espera para verificar as filas novamente se ela estiver vazia
#define EVER ;;


/////////DEFINE PROCESSO//////////
typedef struct Processo{
	int my_pid;
	int nivel_corrente;
}processo;

typedef struct priority_queue{
	int tempo_cota;
	Fila *fila_Prioridade;
}nivel_prioridade;

typedef struct Escalonador{
	nivel_prioridade *nivel_1, *nivel_2, *nivel_3;
	processo *ativo;
	Fila *processos_io;
	int cpu_bound;
	int terminou;
	int cota;
}escalonador;

void tratador_termino_filho(int signal);
void tratador_interpretador(int signal);
void recebe_processo();
void tratador_w4IO(int signal);
void aumenta_prioridade(processo* proc);
void diminui_prioridade(processo* proc);
void test();
escalonador *escal;
int main(void){
	//////////////INICIA FILAS////////
	Fila *fila_Prioridade_1 = fila_cria();
	Fila *fila_Prioridade_2 = fila_cria();
	Fila *fila_Prioridade_3 = fila_cria();
	Fila *ios = fila_cria();
	//////////////////////////////////

	////////INICIA NIVEL_PRIORIDADE///
	nivel_prioridade *fila_1,*fila_2,*fila_3;

	fila_1 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
	fila_1->tempo_cota = DELTA1;
	fila_1->fila_Prioridade = fila_Prioridade_1;

	fila_2 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
	fila_2->tempo_cota = DELTA2;
	fila_2->fila_Prioridade = fila_Prioridade_2;

	fila_3 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
	fila_3->tempo_cota = DELTA3;
	fila_3->fila_Prioridade = fila_Prioridade_3;
	//////////////////////////////////
	
	/////DECLARAÇÕES//////////////////
	int my_pid = getpid();
	signal(SIGCONT,tratador_interpretador);
	signal(SIGUSR1,tratador_w4IO);
	signal(SIGCHLD,tratador_fimIO);
	signal(SIGUSR2,tratador_termino_filho);
	//////////////////////////////////	
	
	/////INICIA ESCALONADOR///////////
	/*escalonador */escal = (escalonador*)malloc(sizeof(escalonador));
	escal->nivel_1 = fila_1;
	escal->nivel_2 = fila_2;
	escal->nivel_3 = fila_3;
	escal->processos_io = ios;
	escal->cota=0;
	//////////////////////////////////	

	///LOOP ESCALONADOR///////////////
	for(EVER){
		if(!fila_vazia(escal->nivel_1->fila_Prioridade)){
			escal->ativo = (processo*)fila_retira(escal->nivel_1->fila_Prioridade);
			escal->cota = escal->nivel_1->tempo_cota;
		}
		else if(!fila_vazia(escal->nivel_2->fila_Prioridade)){
			escal->ativo = (processo*)fila_retira(escal->nivel_2->fila_Prioridade);
			escal->cota = escal->nivel_2->tempo_cota;
		}
		else if(!fila_vazia(escal->nivel_3->fila_Prioridade)){
			escal->ativo = (processo*)fila_retira(escal->nivel_3->fila_Prioridade);
			escal->cota = escal->nivel_3->tempo_cota;
		}
		else {
			printf("\nFilas Vazias, aguardo %d\n",ESPERA);
			test();
			sleep(ESPERA);
			continue;
		}
		///////AGUARDA FILHO//////////////
		escal->cpu_bound = 1;
		escal->terminou = 0;
		kill(escal->ativo->my_pid,SIGCONT);
		sleep(escal->cota);
		if(escal->terminou){
			printf("\n#######################\nProcesso: %d terminou\n#######################",escal->ativo->my_pid);
			free(escal->ativo);
		}
		else{
			if(escal->cpu_bound){
				kill(escal->ativo->my_pid,SIGSTOP);
				diminui_prioridade(escal->ativo);
			}
			else{
				fila_insere(escal->processos_io,escal->ativo);
			}
		}
		//////////////////////////////////
	}
	//////////////////////////////////
	return 0;
}

//////MODIFICA PRIORIDADE/////////
void diminui_prioridade(processo *proc){
	switch(proc->nivel_corrente){
		case NIVEL1:
			proc->nivel_corrente = NIVEL2;
			fila_insere((escal->nivel_2->fila_Prioridade),proc);
		break;
		case NIVEL2:
			proc->nivel_corrente = NIVEL3;
			fila_insere((escal->nivel_3->fila_Prioridade),proc);
		
		break;
		case NIVEL3:
			proc->nivel_corrente = NIVEL3;
			fila_insere((escal->nivel_3->fila_Prioridade),proc);
		break;
	}
}
void aumenta_prioridade(processo *proc){
	switch(proc->nivel_corrente){
		case NIVEL1:
			proc->nivel_corrente = NIVEL1;
			fila_insere((escal->nivel_1->fila_Prioridade),proc);
		break;
		case NIVEL2:
			proc->nivel_corrente = NIVEL1;
			fila_insere((escal->nivel_1->fila_Prioridade),proc);
		
		break;
		case NIVEL3:
			proc->nivel_corrente = NIVEL2;
			fila_insere((escal->nivel_2->fila_Prioridade),proc);
		break;
	}
}
//////////////////////////////////

/*escalonador* pede_escalonador(){
	return ;
}*/

//////FUNÇÃO PARA INTERPRETADOR///
void recebe_processo(){
	int pid;
	int my_pid;

	int fpFIFO_nome, fpFIFO_tam, fpFIFO_tempos;
	int tam;
	int *exect;
	char name[MAX_BUF];
	char params[MAX_BUF];

/*	int fd;
	char *myfifo = "/tmp/myfifo";
	char buf[1024];*/

	if((pid=fork())!=0){	//PAI
		processo *new_processo = (processo*) malloc(sizeof(processo));
		new_processo->my_pid = pid;
		new_processo->nivel_corrente = NIVEL1;
		fila_insere((escal->nivel_1->fila_Prioridade),new_processo);
	}
	else if(pid == 0){
		my_pid = getpid();
		signal(SIGUSR1,SIG_DFL);
		signal(SIGUSR2,SIG_DFL);
		signal(SIGCHLD,SIG_DFL);
		signal(SIGCONT,SIG_DFL);
		//LOOP do FILHO
		/*fd = open(myfifo, O_RDONLY);
		read(fd, buf, 1024);
		printf("Received: %s \n", buf);
		close(fd);*/
		
	    fpFIFO_nome = abre_fifo_read(fpFIFO_nome, FIFO_nome); //abre FIFO
		read(fpFIFO_nome, name, sizeof(MAX_BUF)); //le o nome do programa e o poe no vetor name
		printf("%s\n", name);

		fpFIFO_tam = abre_fifo_read(fpFIFO_tam, FIFO_tam);
		read(fpFIFO_tam, &tam, sizeof(int)); //le o tam do vetor exect no interpretador pra criar um igual aqui e o poe na var tam name
		printf("%d\n", tam);

		fpFIFO_tempos = abre_fifo_read(fpFIFO_tempos, FIFO_tempos);
		exect = (int *)malloc((tam*sizeof(int)));
		/*for (int i=0; i<tam; i++){
			read(fpFIFO_tempos, &exect[1], sizeof(int)); //le cada indice do vetor exect no interpretador e os poe no vetor exect
			printf("%d\n",exect[1]);
		}*/
		
		//read adaptado
		read(fpFIFO_tempos, params, sizeof(MAX_BUF)); 
		
		//fecha os FIFOs
		close(fpFIFO_nome); 
		close(fpFIFO_tam);
		close(fpFIFO_tempos);
	
		execv(name,params,0);
		
	}
	

}
//////////////////////////////////

///////TRATADORES DE SINAL////////
//trata o filho estar "waiting for I/O"
//indica que o filho terminou antes que o pai, ao mesmo tempo despertando-o do sono
void tratador_w4IO(int signal){
	escal->cpu_bound = 0;
}
void tratador_fimIO(int signal){
	
	aumenta_prioridade((processo*)(escal->processos_io));
}

void tratador_termino_filho(int signal){
	escal->terminou = 1;
}
void tratador_interpretador(int signal){
	recebe_processo();
}

void test(){
	int fpFIFO_nome, fpFIFO_tam, fpFIFO_tempos;
	int tam;
	int *exect;
	char name[MAX_BUF];
    fpFIFO_nome = abre_fifo_read(fpFIFO_nome, FIFO_nome); //abre FIFO
	read(fpFIFO_nome, name, sizeof(MAX_BUF)); //le o nome do programa e o poe no vetor name
	printf("%s\n", name);

	fpFIFO_tam = abre_fifo_read(fpFIFO_tam, FIFO_tam);
	read(fpFIFO_tam, &tam, sizeof(int)); //le o tam do vetor exect no interpretador pra criar um igual aqui e o poe na var tam name
	printf("%d\n", tam);

	fpFIFO_tempos = abre_fifo_read(fpFIFO_tempos, FIFO_tempos);
	exect = (int *)malloc((tam*sizeof(int)));
	for (int i=0; i<tam; i++){
		read(fpFIFO_tempos, &exect[1], sizeof(int)); //le cada indice do vetor exect no interpretador e os poe no vetor exect
		printf("%d\n",exect[1]);
	}
	//fecha os FIFOs
	close(fpFIFO_nome); 
	close(fpFIFO_tam);
	close(fpFIFO_tempos);
}
