/*
    Sistemas Operativos
    Autores: Carlos Nevárez, Uriel Cabrera
*/

# include "fshell.h"

int main(int argc, char *argv[]){
    int i;

	int tuberias, redirecciones;
	char *comandos[argc+1];
	char comando[100];
	//printf(" Ejecutare los comandos: \n");

	/* Bucle que recorre todo el arreglo de argumentos para quitar el nombre del programa*/
	for(i=0; i<=argc; i++){
		comandos[i] = argv[i+1];
		// Evalua si tiene tuberias
		if(tieneTuberias(comandos[i])){
			tuberias = 1;
		}
		// Evalua si tiene redirecciones
		if(tieneRD(comandos[i]) || tieneRI(comandos[i])){
			redirecciones = 1;
		}
		printf("\t > %s\n", argv[i]);
		strcat(comando, argv[i]);
		strcat(comando, " ");
	}

	printf("\n EL comando es %s", comando);
	//execvp(comandos[0], comandos);
	//perror("\n Error en excec \n");	
	printf("\n");

	return 0;
}