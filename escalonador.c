//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include "escalonador.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "fila.h"
#define DELTA1 1
#define DELTA2 2
#define DELTA3 4
#define ESPERA 10	//Quanto o escalonador espera para verificar as filas novamente se ela estiver vazia
#define EVER ;;


/////////DEFINE PROCESSO//////////
typedef struct Processo{
	int my_pid;
}processo;

typedef struct priority_queue{
	int tempo_cota;
	Fila *fila_Prioridade;
}nivel_prioridade;

typedef struct Escalonador{
	nivel_prioridade *nivel_1, *nivel_2, *nivel_3;
	processo *ativo;
	int cpu_bound;
	int terminou;
	int cota;
}escalonador;

void tratador_termino_filho(int signal);
void recebe_processo(int tam, int *raj);
void tratador_w4IO(int signal);
void aumenta_prioridade();
void diminui_prioridade();
escalonador *escal;
int main(void){
	//////////////INICIA FILAS////////
	Fila *fila_Prioridade_1 = fila_cria();
	Fila *fila_Prioridade_2 = fila_cria();
	Fila *fila_Prioridade_3 = fila_cria();
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
	signal(SIGUSR1,tratador_w4IO);
	signal(SIGUSR2,tratador_termino_filho);
	//////////////////////////////////	
	
	/////INICIA ESCALONADOR///////////
	/*escalonador */escal = (escalonador*)malloc(sizeof(escalonador));
	escal->nivel_1 = fila_1;
	escal->nivel_2 = fila_2;
	escal->nivel_3 = fila_3;
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
			printf("\nFilas Vazias, aguardo %ds",ESPERA);
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
				diminui_prioridade(escal);
			}
			else{
				aumenta_prioridade(escal);
			}
		}
		//////////////////////////////////
	}
	//////////////////////////////////
	return 0;
}

//////MODIFICA PRIORIDADE/////////
void diminui_prioridade(escalonador *escal){
	switch(escal->cota){
		case DELTA1:
			fila_insere((escal->nivel_2->fila_Prioridade),escal->ativo);
		break;
		case DELTA2:
			fila_insere((escal->nivel_3->fila_Prioridade),escal->ativo);
		
		break;
		case DELTA3:
			fila_insere((escal->nivel_3->fila_Prioridade),escal->ativo);
		break;
	}
}
void aumenta_prioridade(escalonador *escal){
	switch(escal->cota){
		case DELTA1:
			fila_insere((escal->nivel_1->fila_Prioridade),escal->ativo);
		break;
		case DELTA2:
			fila_insere((escal->nivel_1->fila_Prioridade),escal->ativo);
		
		break;
		case DELTA3:
			fila_insere((escal->nivel_2->fila_Prioridade),escal->ativo);
		break;
	}
}
//////////////////////////////////

/*escalonador* pede_escalonador(){
	return ;
}*/

//////FUNÇÃO PARA INTERPRETADOR///
void recebe_processo(int tam, int *raj){
	int pid;
	int my_pid;
	if((pid=fork())!=0){	//PAI
		processo *new_processo = (processo*) malloc(sizeof(processo));
		new_processo->my_pid = pid;
		fila_insere((escal->nivel_1->fila_Prioridade),new_processo);
	}
	else if(pid == 0){
		my_pid = getpid();
		signal(SIGUSR1,SIG_DFL);
		signal(SIGUSR2,SIG_DFL);
		raise(SIGSTOP);
		//LOOP do FILHO
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
	

}
//////////////////////////////////

///////TRATADORES DE SINAL////////
//trata o filho estar "waiting for I/O"
//indica que o filho terminou antes que o pai, ao mesmo tempo despertando-o do sono
void tratador_w4IO(int signal){
	escal->cpu_bound = 0;
}

void tratador_termino_filho(int signal){
	escal->terminou = 1;
}
