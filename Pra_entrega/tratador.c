#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "tratador.h"


void separador_input(char *input, char word[3][100], int identificador){ //funcao para separar as palavras da string, ou seja separa as palavras dividias por espaco
    int i= 0, j=0, k=0;
    input[strlen(input) - 1] = '\0';
    if (input[0] == '\0'){
        //checa se o input foi vazio ou invalido
        printf("Input invalido.\n");
        exit(-1);
    }

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
