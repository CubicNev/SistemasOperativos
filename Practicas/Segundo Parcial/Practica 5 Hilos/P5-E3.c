/*
    Autor: Carlos Nevárez
    Sistemas Operativos - 24/Marzo/2020
    Programa que ocupa dos hilos para manipular una variable global (inicia en cero). Uno la aumente B unidades y otro la aumenta A unidades
*/

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

int global = 0;

void* hilo_aumenta(void* A){
    //printf("\n Aumenta %d\n", *(int *)A);
    global = global + *(int *)A;
    //printf("\n A: GLOBAL: %d", global);
}

void* hilo_disminuye(void* B){
    //printf("\n Disminuye %d\n", *(int *)B);
    global = global - *(int *)B;
    //printf("\n B: GLOBAL: %d", global);
}

int main(){
    pthread_t h_A, h_B;
    int err1, err2,
        A, B;

    printf("\n Ingresa A: "); scanf("%d", &A);
    printf("\n Ingresa B: "); scanf("%d", &B);

    err1 = pthread_create(&h_A, NULL, hilo_aumenta, &A);
    if(err1){
       printf("\n Error en pthread_create HA %i\n", err1);
        exit(-1); 
    }
    err2 = pthread_create(&h_B, NULL, hilo_disminuye, &B);
    if(err1){
       printf("\n Error en pthread_create HA %i\n", err1);
        exit(-1); 
    }

    pthread_join(h_A, NULL);
    pthread_join(h_B, NULL);
    printf("\n El resultado de la variable es: %d\n", global);
    printf("\n FIN\n");

    return 0;
}