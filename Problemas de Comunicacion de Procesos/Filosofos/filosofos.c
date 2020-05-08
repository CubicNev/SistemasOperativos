/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 1/Abril/2020
    Programa que simula el problemaa de los filosofos comelones
*/
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h> //Biblioteca para semaforos en Linux

# define NFILOSOFOS 5
/* Estados de un filosofo */
# define PENSANDO 0
# define HAMBRIENTO 1
# define COMIENDO 2

int estado_filosofo[NFILOSOFOS];
sem_t s_filosofos[NFILOSOFOS];
pthread_mutex_t sem = PTHREAD_MUTEX_INITIALIZER;

void pensar(int fil){
    printf("\n Soy el filosofo %i, Voy a pensar\n", fil);
    sleep(2);
}

void comer(int fil){
    printf("\n Soy el filosofo %i, Voy a comer\n", fil);
    sleep(3);
}

int filosofo_derecho(int fil){
    if(fil==0)
        return NFILOSOFOS - 1;
    else
        return fil - 1;
}

int filosofo_izquierdo(int fil){
    if(fil==(NFILOSOFOS - 1))
        return 0;
    else
        return fil + 1;
}

void tenedores_disponibles(int fil){
    if(estado_filosofo[fil]==HAMBRIENTO &&
        estado_filosofo[filosofo_derecho(fil)] != COMIENDO &&
        estado_filosofo[filosofo_izquierdo(fil)] != COMIENDO) {
            estado_filosofo[fil] = COMIENDO;
            printf("\n Soy el filosofo %i. Estoy tomando mis tenedores.\n", fil);
            sem_post(&s_filosofos[fil]); // up: para dejar que coma
        } else {
            printf("\n Soy el filosofo %i. Voy a esperar por mis tenedores.\n", fil);
        }
}

void tomar_tenedores(int fil){
    pthread_mutex_lock(&sem); // down para region critica
    estado_filosofo[fil] = HAMBRIENTO;
    printf("\n Soy el filosofo %i. Estoy hambriento, intentaré tomar mis tenedores.\n", fil);
    tenedores_disponibles(fil);
    pthread_mutex_unlock(&sem); // up para region critica
    sem_wait(&s_filosofos[fil]); // down: 
}

void dejar_tenedores(int fil){
    pthread_mutex_lock(&sem); // down para region critica

    estado_filosofo[fil] = PENSANDO;
    printf("\n Soy el filosofo %i. Estoy satisfecho, dejaré mis tenedores.\n", fil);

    tenedores_disponibles(filosofo_derecho(fil));
    tenedores_disponibles(filosofo_izquierdo(fil));

    pthread_mutex_unlock(&sem); // up para region critica
}

void* vida_filosofo(void *arg){
    int fil = *(int *)arg; // Cast para pasar el id de los hilos
    while(1){
        pensar(fil);
        tomar_tenedores(fil);
        comer(fil);
        dejar_tenedores(fil);
    }
}

int main(){

    pthread_t filosofos[NFILOSOFOS];
    int idFil[NFILOSOFOS], i, err;

    for(i=0; i<NFILOSOFOS; i++){
        //Inicializa el arreglo de semaforos de sincronizacion
        sem_init(&s_filosofos[i], 0, 0);
    }

    for(i=0; i<NFILOSOFOS; i++){
        idFil[i] = i;
        err = pthread_create(&filosofos[i], NULL, vida_filosofo, &idFil[i]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=0; i<NFILOSOFOS; i++){
        pthread_join(filosofos[i], NULL);
    }


    return 0;
}