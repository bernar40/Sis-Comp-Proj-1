#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "fifo.h"


void cria_fifo(char *FIFO){
    if(access(FIFO, F_OK) == -1){
        if (mkfifo(FIFO, S_IRUSR | S_IWUSR) != 0){ //Cria o fifo com as devidas permissoes
            printf("Erro ao criar a FIFO: %s\n", FIFO);
            exit (-1);
        }
    }
}

int abre_fifo_write(int fpFIFO, char * FIFO){
    if ((fpFIFO = open(FIFO, O_WRONLY | O_NONBLOCK)) < 0){//Abre o fifo para escrita e bloqueia, ele espera os dados serem recebidos
        printf("Erro ao abrir a FIFO: %s\n", FIFO);
        exit (-1);
        }
    return fpFIFO;
}

int abre_fifo_read(int fpFIFO, char * FIFO){
    if ((fpFIFO = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0){//Abre o fifo para escrita e bloqueia, ele espera os dados serem recebidos
        printf("Erro ao abrir a FIFO: %s\n", FIFO);
        exit (-1);
        }
    return fpFIFO;
}