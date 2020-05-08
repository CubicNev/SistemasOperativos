/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 24/Marzo/2020
    Programa que calcula la multiplicacion de dos números enteros mediante sumas sucesivas. 
    Donde E1 x E2
    Es: E1 + E1 + E1 + ..., E2 veces
    Va a ocupar N hilos, cada hilo va hacer E2/N sumas de E1.
    EL main va sumar el resultado de cada de la suma que hizo cada hilo.
*/

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <pthread.h>
# include <unistd.h>
# include <signal.h>
# include <time.h>
# include <sys/time.h>

int r_sumas = 0;//esta variable sera la encargada de tener el resultado final
int E1;
int E2;
int N;
int n_sumas;

int esPar(int n){
    if(n%2 == 0){
        return 1;
    } else {
        return 0;
    }
}

int abs(int n){
    if(n>0)
        return n;
    else 
        return -n;
}

void* funcion_sumas(void * param){

    int i, suma=0;
    
    //Para medir el tiempo de ejecucion
    struct timeval t, t2;
    int microseg;

    printf("\n Hilo %i: Se va a sumar %i, %i veces.\n", *(int *)param, E1, n_sumas);

    //Inicio del hilo.
    gettimeofday(&t, NULL);
    for(i=0; i<n_sumas; i++){
        suma = suma + E1;
    }
    r_sumas = r_sumas + suma;
    //Fin del programa
    gettimeofday(&t2, NULL);
    microseg = (t2.tv_usec - t.tv_usec) + ((t2.tv_sec - t.tv_sec) * 1000000.0f);
    printf("\n Resultado H%i: %i en %i microsegundos\n", *(int *)param, r_sumas, microseg);

    pthread_exit(NULL);
}

int signo(int e1, int e2, int p){

    if((e1>0 && e2>0) || (e1<0 && e2<0))
        return p;
    else
        return -p;

}

int main(int argc, char* argv[]){
    int n1, n2,
        error,
        i,
        *salida; //Variable para recuperar el resultado de la suma del hilo
    
    //Para medir el tiempo de ejecucion
    struct timeval t, t2;
    int microseg;

    if(argc != 4){
        printf("\n Debes ingresar 4 argumentos\n");
        exit(-1);
    }

    //Inicio del programa.
    gettimeofday(&t, NULL);
    n1 = atoi(argv[1]);
    n2 = atoi(argv[2]);
    E1 = abs(n1); 
    E2 = abs(n2); 
    N = atoi(argv[3]); 
    n_sumas = E2 / N;

    pthread_t hilo[N];
    int id[N];

    if(E1 == 0 || E2 == 0){ // Si alguno de los números de multiplicación es cero, el resultado siempre va ser cero
        r_sumas = 0;
    } else {
        if(E2 % N == 0){ //Si se pueden distribuir las sumas uniformemente, no hay problema y todo se ejecuta facil.
            //printf("\n E1 = %i, E2 = %i, N = %i, se van a hacer %i sumas por hilo\n", E1, E2, N, n_sumas);

            for(i=0; i<N; i++){ //Empieza la creacion de hilos
                id[i] = i;
                error = pthread_create(&hilo[i], NULL, funcion_sumas, &id[i]);
                if(error){
                    printf("\n Error en pthread_create %i\n", error);
                    exit(-1);
                }
            }

            for(i=0; i<N; i++){//Para esperar a los hilos
                pthread_join(hilo[i], NULL);
            }
        } else { //Si no es uniforme uno de los hilos debe hacer mas sumas
            //printf("\n E1 = %i, E2 = %i, N = %i, %i hilo(s) van a hacer %i sumas y 1 hilo va a hacer %i sumas \n", E1, E2, N, N-1, n_sumas, n_sumas+(E2 % N));
            for(i=0; i<N-1; i++){ //Crea N-1 Hilos
                id[i] = i;
                error = pthread_create(&hilo[i], NULL, funcion_sumas, &id[i]);
                if(error){
                    printf("\n Error en pthread_create %i\n", error);
                    exit(-1);
                }
            }

            for(i=0; i<N-1; i++){//Espera N-1 Hilos
                pthread_join(hilo[i], NULL);
            }

            //Crear el hilo N con n_sumas+(E2%N)
            n_sumas = n_sumas+(E2%N);
            id[N-1] = N-1;
            error = pthread_create(&hilo[i], NULL, funcion_sumas, &id[i]);
            if(error){
                printf("\n Error en pthread_create %i\n", error);
                exit(-1);
            }

            //Esperar el Hilo N
            pthread_join(hilo[N-1], NULL);
        }
    }

    r_sumas = signo(n1, n2, r_sumas);
    printf("\n El resultado de %i x %i es %i\n", n1, n2, r_sumas);
    //Fin del programa
    gettimeofday(&t2, NULL);
    microseg = (t2.tv_usec - t.tv_usec) + ((t2.tv_sec - t.tv_sec) * 1000000.0f);
    printf("\n El tiempo de ejecución del programa fue: %d microsegundos\n", microseg);
    printf("\n FIN\n");
    return 0;
}