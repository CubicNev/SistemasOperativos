/*
    Autor: Carlos Nevárez
    Fecha: 10-Marzo-2020
    Tarea 3
*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

# define T_MENSAJE 200

void confHijo(int sig){
    printf("\n\t HIJO > Mi padre termino de escribir, comenzare a leer.\n");
}

void confPadre(int sig){
    printf("\n\t PADRE > Mi hijo recibio el mensaje, leere la tuberia2.\n");
}

int main(){
    int pid, adios;
    int tuberia[2], tuberia2[2];
    char mensaje[T_MENSAJE];

    if(pipe(tuberia)==-1){
        perror("\n Error en pipe\n");
        exit(-1);
    }

    if(pipe(tuberia2)==-1){
        perror("\n Error en pipe 2\n");
        exit(-1);
    }

    printf("\n %i, %i\n", tuberia[0], tuberia[1]);
    printf("\n %i, %i\n", tuberia2[0], tuberia2[1]);
    pid = fork();

    if(pid==-1){
        printf("\n Error en fork\n");
        exit(-1);
    } else if(pid==0){ // Hijo
        adios = 1;
        while(adios!=0){
            signal(SIGUSR1, confHijo);
            pause();
            read(tuberia[0], mensaje, sizeof(mensaje)); // El hijo lee la tuberia
            printf("\n HIJO > mensaje leído: %s\n", mensaje); // Despliegue del mensaje
            adios = strcmp(mensaje,"ADIOS\n"); // Compara si el mensaje es ADIOS
            write(tuberia2[1], mensaje, strlen(mensaje)+1);// Escribe en la segunda tuberia
            printf(" HIJO > He escrito en la tuberia2. \n");
            kill(getppid(), SIGUSR1); // Envia señal al padre
        }
        printf("\n HIJO > ADIOS owo\n");
        exit(0);
    } else { // Padre
        adios = 1;
        while(adios != 0){
            printf("\n PADRE > Introduce mensaje: ");
            fgets(mensaje, sizeof(mensaje), stdin); //Recibe el mensaje
            write(tuberia[1], mensaje, strlen(mensaje)+1); //Escribir en la tubería

            kill(pid, SIGUSR1);// Envia señal al hijo para que lea la tuberia
            signal(SIGUSR1, confPadre);
            pause();
            read(tuberia2[0], mensaje, sizeof(mensaje));// Lee lo que el hijo escribio en la segunda tuberia
            printf("\n PADRE > He leido la tuberia2. \n");
            adios = strcmp(mensaje, "ADIOS\n"); //Compara si el mensaje es ADIOS
        }
        printf("\n PADRE > ADIOS uwu\n");
        wait(NULL);
    }

 return 0;
}