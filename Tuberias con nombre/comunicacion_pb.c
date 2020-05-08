/*
    Autor: Carlos Nevárez - Sistemas Operativos
    Programa que funciona como una radio, usando tuberias con nombre.

*/
# include <stdio.h>
# include <sys/types.h> //Libreria para tuberia
# include <sys/stat.h> //Libreria para tuberia
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdlib.h>

# define T_MENSAJE 200

int main() {

    int ab, ba, da_ab, da_ba, cambio, cambio_fuera, bandera=0;
    char mensaje[T_MENSAJE];

    da_ab = open("fifo_ab",O_WRONLY);//Nos regresa el descriptor de archivos en donde escribe este archivo
    da_ba = open("fifo_ba",O_RDONLY);

    do {

        if(bandera=0){

            do{
                printf("\nPROCESO B (OwO)\n");
                fgets(mensaje,sizeof(mensaje),stdin);
                write(da_ba, mensaje, strlen(mensaje)+1);

                cambio = strcmp(mensaje,"CAMBIO\n");
                cambio_fuera = strcmp(mensaje, "CAMBIO Y FUERA\n");
            } while(cambio!=0 && cambio_fuera!=0);

            if(cambio==0)
                bandera = 1;

        } else {
            do{
                read(da_ab, mensaje, sizeof(mensaje));
                printf("\nPROCESO A (UwU) %s\n", mensaje);
                cambio = strcmp(mensaje,"CAMBIO\n");
                cambio_fuera = strcmp(mensaje, "CAMBIO Y FUERA\n");
            } while(cambio!=0 && cambio_fuera!=0);

            if(cambio==0)
                bandera = 0;
        }
        
    } while(cambio_fuera!=0);

 return 0;
}