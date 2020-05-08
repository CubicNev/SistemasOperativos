/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 3/Abril/2020
    Programa que resuelve el problemaa de los lectores y escritores.
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h> 

# define NLECTORES 11
# define NESCRITORES 6
# define TCADENA 100

int num_lector = 0;
FILE *bd;

pthread_mutex_t mutexArchivo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexNl = PTHREAD_MUTEX_INITIALIZER;

void leer_archivo(int lector){
    char cadena[TCADENA];
    printf("\n LECTOR %i leyendo:\n", lector);
    bd = fopen("base_datos.txt", "r");

    while (fgets(cadena, sizeof(cadena), bd)) {
        fprintf(stdout, "\n\t L%i: %s\n", lector, cadena);
    }

    fclose(bd);
    printf("\n El lector %i termino de leer.\n", lector);
}

void escribir_archivo(int escritor){
    char cadena[TCADENA];
    int seguir;
    
    bd = fopen("base_datos.txt", "a");
    printf("\n ESCRITOR %i: ", escritor);
    //scanf("%s", cadena);
    fgets(cadena, sizeof(cadena), stdin);
    fputs(cadena, bd);
    fclose(bd);

    printf("\n El escritor %i termino de escribir.\n", escritor);
}

void* lector(void *arg){
    // Entra un lector
    int lec = *(int *)arg;
    /* Inicio zona critica para num_lector */
    pthread_mutex_lock(&mutexNl);// down para restringir acceso
    
    num_lector++;
    printf("\n Entro el lector %i, hay %i lectores en la BD\n", lec, num_lector);
    if(num_lector == 1) // Solo nos interesa cuando entra el primer lector 
        pthread_mutex_lock(&mutexArchivo); // down para que no entre un escritor

    pthread_mutex_unlock(&mutexNl);// up para liberar acceso
    /* Fin zona critica para num_lector */

    leer_archivo(lec);
    sleep(lec); // Sleep para comprobar funcionamiento
    // Sale un lector

    /* Inicio zona critica para num_lector */
    pthread_mutex_lock(&mutexNl);// down para restringir acceso

    num_lector--;
    printf("\n Salio el lector %i, hay %i lectores en la BD\n", lec, num_lector);
    if(num_lector == 0) // Solo nos intera cuando ya no hay lectores
        pthread_mutex_unlock(&mutexArchivo); // up para despertar a un escritor

    pthread_mutex_unlock(&mutexNl);// up para liberar acceso
    /* Fin zona critica para num_lector */
}

void* escritor(void *arg){
    int esc = *(int *)arg;
    // Entra un escritor
    pthread_mutex_lock(&mutexArchivo); // down para ver si puede escribir en la BD
    printf("\n Entro el escritor %i\n", esc);

    escribir_archivo(esc);

    // Sale un escritor
    printf("\n Salio el escritor %i\n", esc);
    pthread_mutex_unlock(&mutexArchivo); // up para marcar que dejo de escribit en la BD
    pthread_exit(0);
}

int main(){
    pthread_t lectores[NLECTORES],
              escritores[NESCRITORES];
    int idL[NLECTORES],
        idE[NESCRITORES],
        err, cnt_e, cnt_l, i;

    // Ejecuta 2 escritores (0 a 1)
    for(cnt_e=0; cnt_e<2; cnt_e++){
        idE[cnt_e] = cnt_e;
        err = pthread_create(&escritores[cnt_e], NULL, escritor, &idE[cnt_e]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=0; i<2; i++){
        pthread_join(escritores[i], NULL);
    }

    // Ejecuta 5 lectores (0 a 4)
    for(cnt_l=0; cnt_l<5; cnt_l++){
        idL[cnt_l] = cnt_l;
        err = pthread_create(&lectores[cnt_l], NULL, lector, &idL[cnt_l]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=0; i<5; i++){
        pthread_join(lectores[i], NULL);
    }
    
    // Ejecuta 2 escritores (2 a 3)
    for(;cnt_e<4; cnt_e++){
        idE[cnt_e] = cnt_e;
        err = pthread_create(&escritores[cnt_e], NULL, escritor, &idE[cnt_e]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=2; i<4; i++){
        pthread_join(escritores[i], NULL);
    }

    // Ejecuta 5 lectores (5 a 9)
    for(; cnt_l<10; cnt_l++){
        idL[cnt_l] = cnt_l;
        err = pthread_create(&lectores[cnt_l], NULL, lector, &idL[cnt_l]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=5; i<10; i++){
        pthread_join(lectores[i], NULL);
    }

    // Ejecuta 2 escritores (4 a 5)
    for(;cnt_e<6; cnt_e++){
        idE[cnt_e] = cnt_e;
        err = pthread_create(&escritores[cnt_e], NULL, escritor, &idE[cnt_e]);
        if(err){
            printf("\n Error en pthread_create %i\n", err);
            exit(-1);
        }
    }

    for(i=4; i<6; i++){
        pthread_join(escritores[i], NULL);
    }

    // Ejecuta 1 lector (10)
    idL[10] = 10;
    err = pthread_create(&lectores[10], NULL, lector, &idL[10]);
    if(err){
        printf("\n Error en pthread_create %i\n", err);
        exit(-1);
    }

    pthread_join(lectores[10], NULL);
    
    return 0;
}