/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 27/Marzo/2020
    Programa que usa dos procesos para imprimir "hola mundo", pero de manera sincronizada usando semaforos
*/
# include <stdio.h>
# include <unistd.h>
# include <pthread.h> // Para el manejo de concurrencia con hilos
# include <semaphore.h> // Libreria para los semaforos para linux
// # include <dispatch/dispatch.h> Para distribuciones de MacOS

sem_t semaforo;
//dispatch_semaphore_t semaforo;

void hola(void){
    printf(" hola");

    /* Va el UP para que abra un lugar en la cola o ejecute el proceso en cola.*/
    sem_post(&semaforo); // Recibe un apuntador al semaforo 
    //dispatch_semaphore_signal(semaforo);
    pthread_exit(0); // Termina el hilo
}

void mundo(void){
    /* Se coloca un DOWN para que si este hilo es el primero, entonces se bloquea */
    sem_wait(&semaforo); // Recibe un apuntador al semáforo.
    //dispatch_semaphore_wait(semaforo, DISPATCH_TIME_FOREVER); La bandera indica que se queda esperando hasta que lo despierten

    printf(" mundo\n");
    pthread_exit(0); // Termina el hilo
}

int main(){
    pthread_t h1, h2; // Declaro hilos.

    // Inicializo los semáforos
    sem_init(&semaforo, 0/*Indica que trabaja con hilos*/, 0/*Valor de inicializacion*/);
    //semaforo = dispatch_semaphore_create(0); EN los parametros va el valor de inicializacion

    // Se manda a crear a los hilos
    pthread_create(&h1, NULL, (void *)mundo, NULL);
    sleep(1); // Para aseguranos que si estan sincronizados, primero se ejecuta h1
    pthread_create(&h2, NULL, (void *)hola, NULL);

    // Esperamos a los hilos
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);

    return 0;
}
