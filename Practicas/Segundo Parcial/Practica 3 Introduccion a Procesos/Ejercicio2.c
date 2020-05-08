#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int suma(int a, int b){return (a+b);}
int resta(int a, int b){return (a-b);}
int multiplicacion(int a, int b){return (a*b);}
float division(float a, float b){return (a/b);}

int operaciones(int i, int a, int b){
	if(i==0){
		printf("\n HIJO\t> %d + %d = %d", a, b, suma(a,b));
		return 0;
	} else if(i==1) {
		printf("\n HIJO\t> %d - %d = %d", a, b, resta(a,b));
		return 0;
	} else if(i==2) {
		printf("\n HIJO\t> %d * %d = %d", a, b, multiplicacion(a,b));
		return 0;
	} else if(i==3) {
		if(b!=0){
			printf("\n HIJO\t> %d / %d = %f", a, b, division(a,b));
			return 0;
		} else{
			printf("\n HIJO\t> Fallo en la division. No la voa' hacer.");
			return 1;
		}
	} else {
		printf("\n");
	}
}

int main(int argc, char * argv[]) {
	int i, pid, pidw, status, vt;
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	for(i=0; i<4; i++){
		pid = fork();
		if(pid==-1) {
			perror("Error en la llamada fork");
			exit(-1);		
		} else if(pid==0) {//Codigo hijo
			printf(" \n HIJO\t> Hola soy el proceso hijo %i", i);
			printf(" \n HIJO\t> MI pid es %i", getpid());
			printf(" \n HIJO\t> El pid de mi padre es: %i", getppid());
			vt = operaciones(i,a,b);
			exit(vt);
			break; //Hace que solo exista 1 padre
		} else {//Codigo padre
			pidw = wait(&status);
			printf(" \n PADRE\t> Hola soy el proceso padre");
			printf(" \n PADRE\t> Mi pid es: %i", getpid());
			printf(" \n PADRE\t> El pid de mi hijo es %i termino con estado %i", pid, WEXITSTATUS(status));	
			
		}
		printf("\n");
	}

return 0;
}
