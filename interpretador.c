//interpretador
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

/*
como executar o programa
exec <programa8exec81> (2, 10, 4)  // rajadas de 2, 10, 4 segundos, interrompidas por I/O
exec <programa8exec81> (20, 20, 2) // rajadas de 20, 20, e 2 segundos, interrompidas por I/O
*/

void separador_input(char *input, char word[3][100]){
    int i= 0, j=0, k=0;
    input[strlen(input) - 1] = '\0';
    if (input[0] == '\0') //checa se o input foi vazio
        printf("Input invalido.\n");
    while (input[i] != '\0') {
        if (input[i] == ' ' && k<2) { // ' ' -> primeiro separador
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

void separador_tempo(char *word, int *exect){
    char str[100];
    char tempo_s[3][100];
    const char s[2] = "(";
    char *token;

    printf("separador tempo word: %s\n", word);
    separador_input(word, tempo_s);
    //printf("t1: %s t2: %s t3: %s\n", tempo_s[0], tempo_s[1], tempo_s[2]);
    token = strtok(tempo_s[0], s); //remove o parenteses do primeiro tempo.
    // printf("t1: %s t2: %s t3: %s\n", token, tempo_s[1], tempo_s[2]);
    exect[0] = atoi(token);
    exect[1] = atoi(tempo_s[1]);
    exect[2] = atoi(tempo_s[2]);
}

int main (void){
    char input[100];
    char word[3][100]; //cada palavra do input vem para ca ------- word[0] = exec | word[1] = nome do programa | word[2] = (2, 10, 4) por exemplo
    char *str;
    int *exect; //execution time - cada elemento eh um tempo em segundos
    exect = (int *)malloc((3*sizeof(int)));
    while (fgets(input, 100, stdin)) //repete sempre pegando input do teclado
    {
        separador_input(input, word);
        separador_tempo(word[2], exect);
        //printf("%s nomedoprograma: %s tempos: %s\n", word[0], word[1], word[2]);
        //printf("t1: %d -- t2: %d -- t3: %d", exect[0], exect[1], exect[2]);

        /* CHAMAR O ESCALONADOR AQUI */
    }
    
    return 0;
}



