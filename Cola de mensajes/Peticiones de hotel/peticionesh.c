/*
    Autor: Carlos Nevárez
    Fecha: 24-04-2020
    Programa que reuelve el como llevar a cabo la gestion de peticiones
    que se hacena un hotel. Son peticiones con prioridad.
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

struct reservacion{
    long peticion;
    int cliente;
    char mensaje[50];
};

int main(){

}