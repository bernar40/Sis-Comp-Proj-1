//interpretador
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include "tratador.h"

/*
como executar o programa
exec <programa8exec81> (2, 10, 4)  // rajadas de 2, 10, 4 segundos, interrompidas por I/O
exec <programa8exec81> (20, 20, 2) // rajadas de 20, 20, e 2 segundos, interrompidas por I/O
*/

int main (int argc, char *argv[]){
    int esc_pid = atoi(argv[1]);
    char input[100];
    char word[3][100]; //cada palavra do input vem para ca ------- word[0] = exec | word[1] = nome do programa | word[2] = (2, 10, 4) por exemplo
    char *str;
    int *exect, *p_tam; //execution time - cada elemento eh um tempo em segundos
    int last;
    int size1, size2, tam;
    int seg_nome, seg_tam, seg_tp;
    char *p_nome, *p_tempos;
   

    printf("Digite seu processo:\n");
    while (fgets(input, 100, stdin)) //repete sempre pegando input do teclado
    {
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


        separador_input(input, word, 2);

        //last = strlen(word[2]);
        //word[2][last] = ')';
        //word[2][last+1] = '\0';

        strcpy(p_nome, word[1]);

        strcpy(p_tempos, word[2]);
        
        printf("Nome: %s ----- tempos: %s\n", p_nome, p_tempos);

        shmdt(p_nome);
        shmdt(p_tempos);
        kill(esc_pid, SIGCONT);

        printf("Processo enviado ao escalonador!\n");
        
        printf("Digite seu novo processo:\n");
    }
    return 0;
}