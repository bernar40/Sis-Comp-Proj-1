//Escalonador
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(void){
	sleep(1);
	printf("%d",getpid());
	sleep(5);
	exit(0);	
}