//Escalonador
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fila.h>
#define DELTA1 1
#define DELTA2 2
#define DELTA3 4
/////////DEFINE PROCESSO//////////
typedef struct Processo{
	int my_pid;
}processo;

//////////////INICIA FILAS////////
Fila *fila_Prioridade_1 = fila_cria (void);
Fila *fila_Prioridade_2 = fila_cria (void);
Fila *fila_Prioridade_3 = fila_cria (void);

//////////////////////////////////

////////INICIA NIVEL_PRIORIDADE///
typedef struct priority_queue{
	int tempo_cota;
	Fila *fila_Prioridade;
}nivel_prioridade;
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

/////INICIA ESCALONADOR///////////
typedef struct Escalonador{
	nivel_prioridade nivel_1, nivel_2, nivel_3;
	processo *ativo;
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
int pid_pai = getpid();

signal(SIGUSR1,tratador_w4IO);
signal(SIGUSR2,tratador_termino_filho);
int cota=0;
processo *ativo;
///LOOP ESCALONADOR///////////////
for(EVER){
	if(!fila_vazia(escal->nivel_1->fila_Prioridade)){
		escal->ativo = (processo*)fila_retira(escal->nivel_1->fila_Prioridade);
		cota = escal->nivel_1->tempo_cota;
	}
	else if(!fila_vazia(escal->nivel_2->fila_Prioridade)){
		escal->ativo = (processo*)fila_retira(escal->nivel_2->fila_Prioridade);
		cota = escal->nivel_2->tempo_cota;
	}
	else if(!fila_vazia(escal->nivel_3->fila_Prioridade)){
		escal->ativo = (processo*)fila_retira(escal->nivel_3->fila_Prioridade);
		cota = escal->nivel_3->tempo_cota;
	}
	else continue;
	///////AGUARDA FILHO//////////////
	//time_t then=time(&then);
	
	kill(ativo->my_pid,SIGCONT);
	sleep(cota);
	//ERRADO, PRECISA SER CAPAZ DE ver que o filho não acabou na cota prevista
	/*waitpid(ativo->my_pid);*/
	//time_t now=time(&now);
	//double diff = difftime(now,then);
	
	
	//////////////////////////////////
}

int pid_pai = getpid();
signal(SIGUSR1,tratador_w4IO);
signal(SIGUSR2,tratador_termino_filho);
void recebe_processo(int tam, int *raj){
	int pid;
	if((pid=fork())!=0){	//PAI
		processo *new_processo = (processo*) malloc(sizeof(processo));
		new_processo->my_pid = pid;
		fila_insere((escal->nivel_1->fila_prioridade),new_processo);
	}
	else if(pid == 0){
		my_pid = getpid();
		signal(SIGUSR1,SIG_DFL);
		signal(SIGUSR2,SIG_DFL);
		raise(SIGSTOP);
		//LOOP do FILHO
		for(int i=0;i<tam;i++){
			for(int j=0;j<raj[i];j++){
				printf("%d\n",my_pid);	//Output espacíficado pelo enunciado
				sleep(1);
			}
			//condição I/O
			raise(SIGSTOP);			//Ativa o WAIT do pai
			sleep(3);
			raise(SIGSTOP);
		}
		//Acabou execução
		kill(getppid(),SIGUSR2);
	}
	exit();
	

}
//trata o filho estar "waiting for I/O"
void tratador_w4IO(int signal){
}

void tratador_termino_filho(int signal){
}

void tratador_tempo_de_IO(int signal){


}
