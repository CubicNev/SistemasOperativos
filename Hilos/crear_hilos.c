/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 20/Marzo/2020
    Se van a crear más de dos hilos y se va a mandar un parametro a la funcion que ejecuta los hijos. 
    Tambien, los hilos devolveran un valor cuando se use join para poder ver cuando terminan. 
*/
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

# define N_HILOS 5

int var = 0; //Variable global, la comparten los hilos

void* funcion_hilo(void * arg){
    int i;
    for(i=0; i<5; i++){
        printf("\n Hilo %d, i: %i, var: %i \n", *(int *)arg, i, var++);
    }
    pthread_exit(arg); //Manda el valor de retorno, en este caso el id del hilo.
}

int main(){
    pthread_t hilo[N_HILOS];
    int error,
        i,
        id[N_HILOS],
        *salida;

    for(i=0; i<N_HILOS; i++){
        id[i] = i;
        error = pthread_create(&hilo[i], NULL, funcion_hilo, &id[i]);
        if(error){
            printf("\n Error en pthread_create %i\n", error);
            exit(-1);
        }
    }

    for(i=0; i<N_HILOS; i++){
        pthread_join(hilo[i], (void **)&salida);
        printf("\n El hilo %i termino con valor %i\n", i, *salida);
    }

    return 0;
}