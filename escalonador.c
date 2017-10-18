//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/queue.h>

/////////DEFINE PROCESSO//////////
typedef struct Processo{
	int my_pid;
	LIST_ENTRY(Processo) entries;
}processo;

//////////////INICIA FILAS////////
STAILQ_HEAD(stailhead, processo) fila_Prioridade_1 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_1);
STAILQ_HEAD(stailhead, processo) fila_Prioridade_2 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_2);
STAILQ_HEAD(stailhead, processo) fila_Prioridade_3 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_3);

typedef struct stailhead StailH;

STAILQ_INIT(&fila_Prioridade_1);
STAILQ_INIT(&fila_Prioridade_2);
STAILQ_INIT(&fila_Prioridade_3);
//////////////////////////////////

////////INICIA NIVEL_PRIORIDADE///
typedef struct priority_queue{
	int tempo_cota;
	StailH *fila_Prioridade;
}nivel_prioridade;
nivel_prioridade *fila_1,*fila_2,*fila_3;

fila_1 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
fila_1->tempo_cota = 1;
fila_1->fila_Prioridade = &fila_Prioridade_1;

fila_2 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
fila_2->tempo_cota = 2;
fila_2->fila_Prioridade = &fila_Prioridade_2;

fila_3 = (nivel_prioridade*)malloc(sizeof(nivel_prioridade));
fila_3->tempo_cota = 4;
fila_3->fila_Prioridade = &fila_Prioridade_3;
//////////////////////////////////

/////INICIA ESCALONADOR///////////
typedef struct Escalonador{
	nivel_prioridade nivel_1, nivel_2, nivel_3;
	/*
	pid_executando
	cota_do_pid_executando
	cota_nivel_atual?
	fila_pid_em_IO? ou 
	*/
}escalonador;

escalonador *escal = (escalonador*)malloc(sizeof(escalonador))
escal->nivel_1 = fila_1
escal->nivel_2 = fila_2
escal->nivel_3 = fila_3
//////////////////////////////////
void recebe_processo(int tam, int *raj);
void tratador_w4IO(int signal);

//int *myRajadas=NULL,tVet=0;
int pai_id = getpid();
int my_pid = getpid();
#define EVER ;;
int main(void){
	for(EVER);
}


int pid_pai = getpid();
signal(SIGUSR1,tratador_w4IO);
signal(SIGUSR2,tratador_termino_filho);
void recebe_processo(int tam, int *raj){
	int pid;
	if((pid=fork())!=0){	//PAI
		processo *new_processo = (processo*) malloc(sizeof(processo));
		new_processo->my_pid = pid;
		STAILQ_INSERT_TAIL((escal->nivel_1->fila_prioridade),new_processo, entries);
	}
	else if(pid == 0){
		my_pid = getpid();
		signal(SIGUSR1,SIG_DFL);
		signal(SIGUSR2,SIG_DFL);
		raise(SIGSTOP);
		//LOOP do FILHO
		for(int i=0;i<tam;i++){
			for(int j=0;j<raj[i];j++){
				printf("%d\n",my_pid);
				sleep(1);
			}
			//condição I/O
			kill(getppid(),SIGUSR1);
		}
		//Acabou execução
		kill(getppid(),SIGUSR2);
	}
	
	

}
//trata o filho estar "waiting for I/O"
void tratador_w4IO(int signal){}

void tratador_termino_filho(int signal){}

