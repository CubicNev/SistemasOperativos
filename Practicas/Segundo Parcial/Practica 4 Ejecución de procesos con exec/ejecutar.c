# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <string.h>

int main(int argc, char *argv[]){
	int i;
	char cadena[128] = "";
	printf("\t Soy ejecutar, Ejecutare: \n");
	for(i=1; i<argc; i++){
		printf("\t\t > %s\n", argv[i]);
		strcat(cadena, argv[i]);
		strcat(cadena, " ");
	}
	printf("\t\t Comando: %s\n", cadena);
	system(cadena);

 return 0;
}
