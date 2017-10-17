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

int main (int argc, char **argv){
    char *str;
    int *exect; //execution time - cada elemento eh um tempo em segundos
    int i;
    exect = (int *)malloc((argc - 1)*sizeof(int));

    for (i=1; i<=argc-1; i++){
        if (i==1)
            str = strtok(argv[i],"( ,");
        else if (i==argc-1)
            str = strtok(argv[i],")");
        else
            str =  strtok(argv[i],",");
        exect[i-1] = atoi(str);
    }

    //Chama o escalonador com execv? ou isso eh o main do escalonador.c?
    
    free(exect);
    return 0;
}