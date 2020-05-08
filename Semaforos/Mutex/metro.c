/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 31/Marzo/2020
    Programa que simula que 10 amigos se esperen en el metro para ir a la biblioteca.
    Problema: Sincronizaar N procesos
*/

# include <stdio.h>
# include <pthread.h> // En esta biblioteca ya vienen los mutex.
# include <semaphore.h> // Biblioteca para semáforos en Linux
# include <unistd.h>

# define NAMIGOS 5

sem_t barrera; // Para sincronizacion.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Para bloquear y desbloquear el acceso a la memoria.

int procesos = 0;

void sincronizar_procesos(void ){
    int i;
    pthread_mutex_lock(&mutex); // down de bloqueo
    procesos++;  
    if(procesos < NAMIGOS){
        pthread_mutex_unlock(&mutex); // up de desbloqueo
        sem_wait(&barrera); // down de sincronizacion
    } else {
        // Ultima persona en llegar, despierta a los demas.
        for(i=0; i<NAMIGOS; i++){
            sem_post(&barrera); // uo de sincronizacion
        }

        procesos = 0;
        pthread_mutex_unlock(&mutex); // up de desbloqueo
    }
}

void *ir_biblioteca(void *arg){
    int id = *(int *)arg;
    sleep(id); //Para ver la sincronizacion más claro
    printf("\n Soy el amigo %i, llegue al metro\n", id);
    sincronizar_procesos();
    printf("\n Soy el amigo %i, vamos hacia la biblioteca UwU\n", id);

    pthread_exit(0);
}

int main(){
    int i, idA[NAMIGOS];
    pthread_t amigos[NAMIGOS];

    sem_init(&barrera, 0, 0);

    for(i=0; i<NAMIGOS; i++){
        idA[i] = i;
    }

    for(i=0; i<NAMIGOS; i++){
        pthread_create(&amigos[i], NULL, ir_biblioteca, &idA[i]);
    }

    for(i=0; i<NAMIGOS; i++){
        pthread_join(amigos[i], NULL);
    }

    return 0;

}


/*
    gcc sincronizar_procesos.c -o sincronizar-procesos -lpthread
*/