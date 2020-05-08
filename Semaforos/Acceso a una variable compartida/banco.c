/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 27/Marzo/2020
    Programa que tiene dos rutinas para simular retisos e ingresos de dinero en un banco. 
    El objetivo es asegurar que solo 1 proceso acceda a la variable compartida a la vez.
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>

int saldo = 200;
sem_t semaforo;

void* retirar_dinero(void* dinero){
    int retiro = *(int *)dinero;

    sem_post(&semaforo); // up
    /* Inicia zona critica */
    if(saldo > retiro){
        int var = saldo;
        var = var - retiro;
        saldo = var;
        printf("\n Haz retirado %i \n", retiro);
    } else {
        printf("\n Tu saldo actual es: %i\n En este momento no puedes retirar %i\n", saldo, retiro);
    }
    
    sem_wait(&semaforo); // down
    pthread_exit(0);
}

void* ingresar_dinero(void* dinero){
    int ingreso = *(int *)dinero;

    sem_post(&semaforo); // up
    /* Inicia zona critica */
    int var = saldo;
    var = var + ingreso;
    saldo = var;
    printf("\n Haz ingresado %i \n", ingreso);

    sem_wait(&semaforo); // down
    pthread_exit(0);
}

int main(){
    pthread_t h1, h2;
    int retiro, ingreso,
        err1, err2;

    printf("\n Tu saldo actual es: %i\n", saldo);

    printf("\n Ingresa el dinero a retirar: "); scanf("%i", &retiro);
    printf("\n Ingresa el dinero a ingresar: "); scanf("%i", &ingreso);

    sem_init(&semaforo, 0, 1);

    err1 = pthread_create(&h1, NULL, ingresar_dinero, &ingreso);
    if(err1){
        printf("\n Error en pthread_create %i\n", err1);
        exit(-1);
    }
    err2 = pthread_create(&h2, NULL, retirar_dinero, &retiro);
    if(err2){
        printf("\n Error en pthread_create %i\n", err2);
        exit(-1);
    }

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);

    printf("\n Tu saldo actual es: %i\n", saldo);

    return 0;
}