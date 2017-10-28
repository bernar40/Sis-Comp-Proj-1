//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "fila.h"
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
void tratador_fimIO(int signal);
void aumenta_prioridade(processo* proc);
void diminui_prioridade(processo* proc);
//void test();
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
	printf("\nPID Escalonador: %d\n", my_pid);
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
			printf("\nPego processo de prioridade NIVEL1");
		}
		else if(!fila_vazia(escal->nivel_2->fila_Prioridade)){

			escal->ativo = (processo*)fila_retira(escal->nivel_2->fila_Prioridade);
			escal->cota = escal->nivel_2->tempo_cota;
			printf("\nPego processo de prioridade NIVEL2");
		}
		else if(!fila_vazia(escal->nivel_3->fila_Prioridade)){
			escal->ativo = (processo*)fila_retira(escal->nivel_3->fila_Prioridade);
			escal->cota = escal->nivel_3->tempo_cota;
			printf("\nPego processo de prioridade NIVEL3");
		}
		else {
			printf("\nFilas Vazias, aguardo %d",ESPERA);
			//test();
			//recebe_processo();
			sleep(ESPERA);
			continue;
		}
		///////AGUARDA FILHO//////////////
		escal->cpu_bound = 1;
		escal->terminou = 0;
		//printf("Enviando SIGCONT para PID: %d\n", escal->ativo->my_pid);
		kill(escal->ativo->my_pid,SIGCONT);
		//kill(escal->ativo->my_pid,SIGUSR2);
		sleep(escal->cota);
		if(escal->terminou){
			printf("\n#######################\nProcesso: %d terminou\n#######################",escal->ativo->my_pid);
			free(escal->ativo);
		}
		else{
			if(escal->cpu_bound){
				printf("\nProcesso %d foi cpu_bound",escal->ativo->my_pid);
				kill(escal->ativo->my_pid,SIGSTOP);
				//kill(escal->ativo->my_pid,SIGUSR1);
				diminui_prioridade(escal->ativo);
			}
			else{
				printf("\nProcesso %d foi io_bound",escal->ativo->my_pid);
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
	printf("\nPrioridade de %d diminuiu",proc->my_pid);
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
	printf("\nPrioridade de %d aumentou",proc->my_pid);
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
	//printf("entrou no recebe");
	int pid;
	int my_pid;
	char *argv[3];
	int tam;
	int seg_nome, seg_tam, seg_tp;
	char *p_nome, *p_tempos, n[100], t[100];
/*	int fd;
	char *myfifo = "/tmp/myfifo";
	char buf[1024];*/

	if((pid=fork())!=0){	//PAI
		//printf("to no pai");
		printf("\nParent PID: %d", getpid());
		processo *new_processo = (processo*) malloc(sizeof(processo));
		new_processo->my_pid = pid;
		new_processo->nivel_corrente = NIVEL1;
		fila_insere((escal->nivel_1->fila_Prioridade),new_processo);
	}
	else if(pid == 0){
		//printf("to no filho");
		my_pid = getpid();
		signal(SIGUSR1,SIG_DFL);
		signal(SIGUSR2,SIG_DFL);
		signal(SIGCHLD,SIG_DFL);
		signal(SIGCONT,SIG_DFL);
		printf("PID do filho: %d\n", my_pid);
	    seg_nome = shmget(3000, 100*sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
	    seg_tp = shmget(3200, 100*sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);

	    if (seg_nome < 0 || seg_tp < 0){
	        puts("erro no shmget");
	        exit(-1);
	    }
	    p_nome = (char *) shmat(seg_nome, 0, 0);
	    p_tempos = (char *)shmat(seg_tp, 0 ,0);

	    if (p_nome == NULL || p_tempos == NULL){
	        puts("erro no shmat");
	        exit(-2);
	    }

		strcpy(n, p_nome);
	    strcpy(t, p_tempos);

	    printf("\nNome: %s ----- tempos: %s", n, t);

	    shmdt(p_nome);
	    shmdt(p_tempos);

	    shmctl (seg_nome, IPC_RMID, 0);
	    shmctl (seg_tp, IPC_RMID, 0);

		argv[0] = n;
		argv[1] = t;
		argv[2] = NULL;

		execv(n, argv);
	}


}

void test(){
    int seg_nome, seg_tp;
    char *p_nome, *p_tempos, n[100], t[100];

    seg_nome = shmget(3000, 100*sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);
    seg_tp = shmget(3200, 100*sizeof(char), IPC_CREAT | S_IRUSR | S_IWUSR);

    if (seg_nome < 0 || seg_tp < 0){
        puts("\nerro no shmget");
        exit(-1);
    }
    p_nome = (char *) shmat(seg_nome, 0, 0);
    p_tempos = (char *)shmat(seg_tp, 0 ,0);

    if (p_nome == NULL || p_tempos == NULL){
        puts("\nerro no shmat");
        exit(-2);
    }

	strcpy(n, p_nome);
    strcpy(t, p_tempos);

    printf("\nNome: %s ----- tempos: %s", n, t);

    shmdt(p_nome);
    shmdt(p_tempos);

    shmctl (seg_nome, IPC_RMID, 0);
    shmctl (seg_tp, IPC_RMID, 0);
}


//////////////////////////////////

///////TRATADORES DE SINAL////////
//trata o filho estar "waiting for I/O"
//indica que o filho terminou antes que o pai, ao mesmo tempo despertando-o do sono
void tratador_w4IO(int signal){
	printf("\nrecebido sinal W4IO");
	escal->cpu_bound = 0;
}
void tratador_fimIO(int signal){
	printf("\nrecebido sinal fim IO");
	aumenta_prioridade((processo*)fila_retira(escal->processos_io));
}

void tratador_termino_filho(int signal){
	escal->terminou = 1;
}
void tratador_interpretador(int signal){
	recebe_processo();
	//test();
}

