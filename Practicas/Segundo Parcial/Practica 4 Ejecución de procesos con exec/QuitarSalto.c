#include<stdio.h>
#include<stdlib.h>

void quitarSalto(char *cad, char *ret){
	printf("ENtro\n");
	for(; *cad != '\n'; cad++, ret++) {
		*ret = *cad;
		printf("`%c`\n", *ret);
	}
	ret--;
}

int main(){
	char *comando[3] = {"ls","-l","-a\n"};
	char ret[10];
	printf(" Primero es: '%s' \n",comando[2]);
	quitarSalto(comando[2], ret);
	printf(" Despues de la funcion es: '%s' \n",ret);
 return 0;
}
