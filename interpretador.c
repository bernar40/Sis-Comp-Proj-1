//interpretador
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
#include "tratador.c"

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
    int *exect; //execution time - cada elemento eh um tempo em segundos
    int tam, i;
    int fpFIFO_nome, fpFIFO_tam, fpFIFO_tempos;
    //remove os arquivos FIFOS para nao dar erro na criacao
    remove(FIFO_nome); 
    remove(FIFO_tam);
    remove(FIFO_tempos);

    cria_fifo(FIFO_nome);
    cria_fifo(FIFO_tam);
    cria_fifo(FIFO_tempos);

    printf("Digite seu processo:\n");
    while (fgets(input, 100, stdin)) //repete sempre pegando input do teclado
    {
        separador_input(input, word, 2);
        //exect = separador_tempo(word[2], &tam);

        /* CHAMAR O ESCALONADOR AQUI */

        fpFIFO_nome = abre_fifo_write(fpFIFO_nome, FIFO_nome);
        write(fpFIFO_nome, word[1], sizeof(word[1]));

        fpFIFO_tam = abre_fifo_write(fpFIFO_tam, FIFO_tam);
        write(fpFIFO_tam, &tam, sizeof(int));

        
        fpFIFO_tempos = abre_fifo_write(fpFIFO_tempos, FIFO_tempos);
        /*for (i = 0; i<tam; i++){
            write(fpFIFO_tempos, &exect[i], sizeof(int));
        }*/
        write(fpFIFO_tempos, word[3], sizeof(word[3]));

        
        close(fpFIFO_tam);
        close(fpFIFO_tempos);
        close(fpFIFO_nome);

        printf("Processo enviado ao escalonador!\n");
        kill(esc_pid, SIGCONT);
        printf("Digite seu novo processo:\n");
    }
    return 0;
}