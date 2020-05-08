/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 24/Marzo/2020
    Programa ejemplifica el uso de hilos independientes
*/

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define NHILOS 10

void funcion_hilos(void){
    printf("\n Hilo %lu \n", pthread_self()); // pthread_self devuelve el identificador del hilo
    pthread_exit(0);
}

int main() {

    int i;
    pthread_attr_t atributos;
    pthread_t hilos[NHILOS];

    //Se inicializan los atributos como independientes
    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_DETACHED);

    for(i=0; i<NHILOS; i++){
        pthread_create(&hilos[i], &atributos, (void *)funcion_hilos, NULL);
    }

    // El hilo principal se suspende 4 segundos
    sleep(4);
    /* Para esperar a que los hilos terminen, de no ser así al terminar el hilo principal, todos los hilos terminarían. */
    return 0;
}