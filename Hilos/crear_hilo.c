/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 20/Marzo/2020
    Siguiendo de proceso_normal, este codigo crea un hilo para ejecutar la funcion b   

    Al compilar se agrega la directiva -lpthread para que vincule la biblioteca de los hilos
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h> //Para sleep
# include <pthread.h> //Para hilos

void funcion_a(void){
    int i;
    for(i=0; i<5; i++){ //Va a imprimir 5 veces
        printf("\n Soy la funcion A\n");
        sleep(1);
    }
}

void* funcion_b(void * arg){ //Es apuntador a funcion, que recibe un apuntador a void (por parametros, si no recibe no pasa nda)
    int i;
    for(i=0; i<10; i++){ //Va a imprimir 10 veces
        printf("\n Soy la funcion B\n");
        sleep(1);
    }
}

int main(){
    pthread_t hilo;
    int error;

    /* (direccion del hilo, 
    atributos del hilo, 
    llamada a la rutina que va a ejecutar el hilo es un apuntador a funcion, 
    el NUll aqui significa que la funcion no tiene argumentos) */
    error = pthread_create(&hilo, NULL, funcion_b, NULL); //Capturamos el valor de retorno de la funcion por si devuelve un error

    if(error){ 
        printf("\n Error en pthread_create %i\n", error);
        exit(-1);
    }

    funcion_a(); //Va a ser ejecutada por el hilo principal
    
    //Ahora debemos controlar la sincronizacion.
    pthread_join(hilo, NULL); //Funcion para que espere a que termine de ejcutarse el hilo

    return 0;
}