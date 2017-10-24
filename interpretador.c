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
#include "escalonador.h"
#define FIFO1 "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo1"
#define FIFO2 "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo2"
/*
como executar o programa
exec <programa8exec81> (2, 10, 4)  // rajadas de 2, 10, 4 segundos, interrompidas por I/O
exec <programa8exec81> (20, 20, 2) // rajadas de 20, 20, e 2 segundos, interrompidas por I/O
*/
void separador_input(char *input, char word[3][100]);
void separador_tempo(char *word, int *exect);

int main (void){
    char input[100];
    char word[3][100]; //cada palavra do input vem para ca ------- word[0] = exec | word[1] = nome do programa | word[2] = (2, 10, 4) por exemplo
    char *str;
    int *exect; //execution time - cada elemento eh um tempo em segundos
    int tam, i;
    int fpFIFO1, fpFIFO2;
    remove(FIFO1);
    remove(FIFO2);

    if(access(FIFO1, F_OK) == -1){
        if (mkfifo(FIFO1, S_IRUSR | S_IWUSR) != 0){ //Cria o fifo com as devidas permissoes
            puts("Erro ao criar a FIFO1\n");
            return -1;
        }
    }

    if(access(FIFO2, F_OK) == -1){
        if (mkfifo(FIFO2, S_IRUSR | S_IWUSR) != 0){ //Cria o segundo fifo, um sera para o tamanho e outro para o vetor
            puts("Erro ao criar a FIFO2\n"); 
            return -1;
        }
    }

    exect = (int *)malloc((3*sizeof(int))); //TEM QUE MUDAR ISSO POIS NAO SAO SO 3 TEMPOS

    printf("Digite seu processo:\n");
    while (fgets(input, 100, stdin)) //repete sempre pegando input do teclado
    {

        separador_input(input, word);
        separador_tempo(word[2], exect);

        /* CHAMAR O ESCALONADOR AQUI */
        tam = (sizeof(exect)/sizeof(int)) + 1;
        if ((fpFIFO1 = open(FIFO1, O_WRONLY)) < 0){//Abre o fifo para escrita e bloqueia, ele espera os dados serem recebidos 
            puts("Erro ao abrir a FIFO1\n");
            return -2;
        }
        write(fpFIFO1, &tam, sizeof(tam));

        if ((fpFIFO2 = open(FIFO2, O_WRONLY)) < 0){//Abre o fifo para escrita e bloqueia, ele espera os dados serem recebidos
            puts("Erro ao abrir a FIFO2\n");
            return -2;
        }
        for (i = 0; i<tam+1; i++)
            write(fpFIFO2, &exect[i], sizeof(exect[i]));
        close(fpFIFO1);
        close(fpFIFO2);
        printf("Processo enviado ao escalonador!\n");
        printf("Digite seu novo processo:\n");
            
    }
    return 0;
}



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

    separador_input(word, tempo_s);
    token = strtok(tempo_s[0], s); //remove o parenteses do primeiro tempo.
    exect[0] = atoi(token);
    exect[1] = atoi(tempo_s[1]);
    exect[2] = atoi(tempo_s[2]);
}
