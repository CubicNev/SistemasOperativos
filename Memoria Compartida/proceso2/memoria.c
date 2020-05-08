/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 08/04/2020
    Programa para que dos procesos compartan una variable entera.
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h> // shm = shared memory

int main() {
    key_t llave;
    int shmid, // Para el espacio de memoria, shm de shared memory
        *entero, // Apuntador a entero porque ocupamos una variable entera 
        opcion;

    // Paso 1: Crear la llave
    llave = ftok("/bin/ls", 19);
    if(llave == -1){
        perror("\n Error en ftok\n");
        exit(-1);
    }

    // Paso 2: Crear el espacio de memoria
    shmid = shmget(llave, sizeof(int), IPC_CREAT|0777);
    /*
        Banderas 
            1: sI NO EXITE LA CREA 
            2: Da los permisos
    */
    if(shmid == -1){
        perror("\n Error en shmget\n");
        exit(-1);
    }

    // Paso 3: Vincular
    entero = (int *) shmat(shmid, 0, 0);
    /* 
        0 para que el Kernel se encargue.
        Bandera 0 para lectura y escritura.
        Hacemos el cast para la variable qe vamos a usar
    */
   if(entero == NULL){
       perror("\n Error en shmat\n");
       exit(-1);
    }

    while(1){
        printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        printf("\n 1 Consultar\n 2 Modificar\n 3 Finalizar\n\n Opción >\t");
        scanf("%i", &opcion);
        switch(opcion){
            case 1:
                printf("\n Variable:\t%i", *entero); // Este apuntador es el que esta haciendo el vinculo
                break;
            case 2:
                printf("\n Variable:\t");
                scanf("%i", entero);// Al almacenarla aqui, se va ir directamente a la zona de memoria compartida
                break;
            case 3:
                // Paso 4
                // 4.1 Desvincular la zona de memoria
                shmdt((char *)entero);
                // 4.2 Eliminar la zona de memoria
                shmctl(shmid, IPC_RMID, 0); // IPC_RMID indica que quiere eliminar la zona de memoria
                exit(0); // Terminamos el programa
                break;
            default:
                break;
        }
    }

    return 0;
}
