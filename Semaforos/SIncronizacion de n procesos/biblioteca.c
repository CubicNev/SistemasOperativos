/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 27/Marzo/2020
    Programa que simula que 10 amigos se esperen en el metro para ir a la biblioteca.
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>

# define NP 10 // Numero de personas(procesos que va a esperar)

int procesos = 0; // Contador de personas que van llendo
sem_t barreraP, // Para sincronizar
    s; // Para solo dejar que 1 proceso modifique 'procesos'

void sincronizar_procesos(void){
    int i;
    sem_wait(&s); // down
    /* Inicia región critica*/
    procesos++;
    if(procesos < NP){ // Si no es la ultima persona
        /* Fin de region critica*/
        sem_post(&s); // up
        sem_wait(&barreraP); // down: La persona espera a que llegue la ultima
    } else { //Llego la ultima persona
        for(i=0; i<(NP-1); i++) //Ejecuta todos los procesos en espera
            sem_post(&barreraP); // up
        
        procesos = 0; // Ya llegaron todos, ya no esperan a nadie
        /* Fin de region critica*/
        sem_post(&s);
    }
}

void* ir_biblioteca(void* arg){
    int id = *(int *)arg;
    sleep(id);
    printf("\n Soy el amigo %i, llegue al metro\n", id);
    sincronizar_procesos();
    printf("\n Soy el amigo %i, vamos hacia la biblioteca UwU\n", id);

    pthread_exit(0);
}

int main(){
    int i, idA[NP], error;
    pthread_t amigos[NP];

    sem_init(&barreraP, 0, 0); //Incializa el semaforo de sincronizacion
    sem_init(&s, 0, 1); // Inicializa el semaforo de acceso a region critica

    for(i=0; i<NP; i++){
        idA[i] = i;
        error = pthread_create(&amigos[i], NULL, ir_biblioteca, &idA[i]);
        if(error) {
            printf("\n Error en pthread_create %i", error);
            exit(-1);
        }
    }

    for(i=0; i<NP; i++){
        pthread_join(amigos[i], NULL);
    }

    return 0;
}