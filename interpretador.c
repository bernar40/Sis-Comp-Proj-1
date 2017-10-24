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

/*
como executar o programa
exec <programa8exec81> (2, 10, 4)  // rajadas de 2, 10, 4 segundos, interrompidas por I/O
exec <programa8exec81> (20, 20, 2) // rajadas de 20, 20, e 2 segundos, interrompidas por I/O
*/

void separador_input(char *input, char word[3][100], int identificador);
int *separador_tempo(char *word, int *tam);
int num_espacos(char *str);


int main (void){
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
        exect = separador_tempo(word[2], &tam);

        /* CHAMAR O ESCALONADOR AQUI */

        fpFIFO_nome = abre_fifo_write(fpFIFO_nome, FIFO_nome);
        write(fpFIFO_nome, word[1], sizeof(word[1]));

        fpFIFO_tam = abre_fifo_write(fpFIFO_tam, FIFO_tam);
        write(fpFIFO_tam, &tam, sizeof(int));

        
        fpFIFO_tempos = abre_fifo_write(fpFIFO_tempos, FIFO_tempos);
        for (i = 0; i<tam; i++){
            write(fpFIFO_tempos, &exect[i], sizeof(int));
        }

        
        close(fpFIFO_tam);
        close(fpFIFO_tempos);
        close(fpFIFO_nome);

        printf("Processo enviado ao escalonador!\n");
        printf("Digite seu novo processo:\n");
    }
    return 0;
}

void separador_input(char *input, char word[3][100], int identificador){ //funcao para separar as palavras da string, ou seja separa as palavras dividias por espaco
    int i= 0, j=0, k=0;
    input[strlen(input) - 1] = '\0';
    if (input[0] == '\0') //checa se o input foi vazio
        printf("Input invalido.\n");

    while (input[i] != '\0') {
        if (input[i] == ' ' && k<identificador) { // ' ' -> primeiro separador o identificador eh para quando for 2, separar a entrada do teclado e outro valor para quando for os tempos
            word[k][j] = '\0';
            j = 0;
            k++;
        }
        else {
            word[k][j++] = input[i];
        }
        i++;
    }
    word[k][j] = '\0'; //ultima palavra TEMPO
}

int *separador_tempo(char *word, int *tam){ //funcao para separar a string de tempos -> ex. (2, 10, 4)
    char str[100];
    int i;
    *tam = num_espacos(word);
    char tempo_s[*tam][100];
    int *exect = (int *)malloc(((*tam)*sizeof(int)));
     
    const char s[2] = "(";
    char *token;

    separador_input(word, tempo_s, *tam);
    token = strtok(tempo_s[0], s); //remove o parenteses do primeiro tempo.
    exect[0] = atoi(token); // poe no vetor o primeiro elemento que precisa de mais um tratamentozinho por causa do '('
    for (i=1; i<*tam; i++) // poe no vetor de tempo os valores
        exect[i] = atoi(tempo_s[i]);

    return exect;
}

int num_espacos(char *str){ //funcao para contar os numeros de espacos numa string
    int space;
    for (space = 0; *str; str++){
        if (*str != ' ')
            continue;
        space++;
    }
    return space +1;
}
