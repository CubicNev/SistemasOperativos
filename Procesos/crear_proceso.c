#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	int pid;
	pid = fork();
	
	if(pid==-1) {
		perror("Error en la llamada fork");
		exit(-1);
	} else if(pid==0) {
		printf(" \n HIJO\t> Hola soy el proceso hijo");
		printf(" \n HIJO\t> MI pid es %i", getpid());
		printf(" \n HIJO\t> El pid de mi padre es: %i", getppid());
	} else {
		printf(" \n PADRE\t> Hola soy el roceso padre");
		printf(" \n PADRE\t> Mi pid es: %i", getpid());
		printf(" \n PADRE\t> El pid de mi hijo es: %i", pid);
	}
	printf("\n");
	return 0;
}
