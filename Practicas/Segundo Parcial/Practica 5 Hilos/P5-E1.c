/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 24/Marzo/2020
    Programa que ocupa dos hilos para imprimir cadenas carácter por carácter.
*/
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define T_MENSAJE 30

int var = 0; //Variable global, la comparten los hilos

void* funcion_H1(void * arg){
    char *msj= (char *)arg;
    int i;

    for(i=0; msj[i] != '\0'; i++)
        printf("\n H1: %c\n", msj[i]);
}

void *funcion_H2(void * arg){
    char *msj= (char *)arg;
    int i;

    for(i=0; msj[i] != '\0'; i++)
        printf("\n H2: %c\n", msj[i]);
}

int main(){
    pthread_t H1, H2;
    int errH1, errH2,
        *salida;
    char m1[T_MENSAJE], m2[T_MENSAJE];

    printf("\n Ingresa el mensaje 1: "); //fgets(m1, T_MENSAJE, stdin);
    scanf("%s", m1);
    printf("\n Ingresa el mensaje 2: "); //fgets(m2, sizeof(m2), stdin);
    scanf("%s", m2);
    //printf(" M1: %s M2: %s", m1, m2);

    errH1 = pthread_create(&H1, NULL, funcion_H1, m1);
    errH2 = pthread_create(&H2, NULL, funcion_H2, m2);

    if(errH1){
        printf("\n Error en pthread_create H1 %i\n", errH1);
        exit(-1);
    }
    if(errH2){
        printf("\n Error en pthread_create H2 %i\n", errH2);
        exit(-1);
    }

    pthread_join(H1, NULL);
    pthread_join(H2, NULL);
    printf("\n FIN\n");
    
    return 0;
}