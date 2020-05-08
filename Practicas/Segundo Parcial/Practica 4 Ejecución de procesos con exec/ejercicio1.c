# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>

int main(int argc, char *argv[]){
	int i;
	char *comandos[argc+1];
	//printf(" Ejecutare los comandos: \n");
	for(i=0; i<=argc; i++){
		comandos[i] = argv[i+1];
		//printf("\t > %s\n", argv[i]);
	}
	printf("Ejecucion\t > ");
	execvp(comandos[0], comandos);
	perror("\n Error en excec \n");
	printf("ADIOS\n");	
 return 0;
} 
