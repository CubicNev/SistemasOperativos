#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc, char * argv[]) {
	int i, pid, pidw;
	int numArg = atoi(argv[1]);
	for(i=0; i<numArg; i++){
		pid = fork();
		if(pid==-1) {
			perror("Error en la llamada fork");
			exit(-1);		
		} else if(pid==0) {//Codigo hijo
			printf(" \n HIJO\t> Hola soy el proceso hijo %i", i);
			printf(" \n HIJO\t> MI pid es %i", getpid());
			printf(" \n HIJO\t> El pid de mi padre es: %i", getppid());
			sleep(2);
			break; //Hace que solo exista 1 padre
		} else {//Codigo padre
			pidw = wait(NULL);
			printf(" \n PADRE\t> Hola soy el roceso padre");
			printf(" \n PADRE\t> Mi pid es: %i", getpid());
			printf(" \n PADRE\t> El pid de mi hijo es: %i", pid);
		}
		printf("\n");
	}

return 0;
}
