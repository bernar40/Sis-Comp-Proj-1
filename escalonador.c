//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/queue.h>

void tratador_w4IO(int signal);

/////////DEFINE PROCESSO//////////
typedef Processo{
	int my_pid;
	//int *rajadas; int qtd_rajadas;
	//ou
	//lista de rajadas;
}processo;

//////////////INICIA FILAS////////
STAILQ_HEAD(stailhead, processo) fila_Prioridade_1 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_1);
STAILQ_HEAD(stailhead, processo) fila_Prioridade_2 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_2);
STAILQ_HEAD(stailhead, processo) fila_Prioridade_3 = STAILQ_HEAD_INITIALIZER(fila_Prioridade_3);
typedef struct stailhead StailH;	//int (pid) será temporário, deve armazenar tmb a lista de rajadas 
STAILQ_INIT(&fila_Prioridade_1);
STAILQ_INIT(&fila_Prioridade_2);
STAILQ_INIT(&fila_Prioridade_3);
//////////////////////////////////

////////INICIA NIVEL_PRIORIDADE///
nivel_prioridade *fila_1,*fila_2,*fila_3;
typedef struct priority_queue{
	int tempo_cota;
	StailH *fila_Prioridade;
}nivel_prioridade;

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

int pid_pai = getpid();
signal(SIGUSR1,tratador_w4IO);

void tratador_w4IO(int signal){}

