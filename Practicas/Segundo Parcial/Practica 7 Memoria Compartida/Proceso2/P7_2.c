/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 28/04/2020
    Programa para que dos procesos compartan un arreglo de caracteres, 
    tratando de simular una pila.
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <string.h>

# define TAM 10

typedef char ArrCar[TAM];

int tam(char *cad){
    int i;
    for(i=0; *cad!='\0'; i++, *cad++);
    return i;
}

char top(ArrCar a){
    if(tam(a) > 0)
        return a[tam(a)-1];
    else
        return '\0';
}


void push(ArrCar a, char *ent){
    ArrCar *ret;
    printf(" Se va a ingresar %s.", ent);
    a = strcat(a, ent);
}

void pop(ArrCar a){
    printf("\n Tam actual %li, Se retirara: %c\n", strlen(a), a[strlen(a)-1]);
    a[strlen(a)-1] = '\0';
    printf(" Tam actual %ld", strlen(a));
}

int main(){
    
    key_t llave;
    int shmid, opcion;
    ArrCar *pila;
    char *in;

    //Paso 1
    llave = ftok("/bin/ls", 19);
    if(llave == -1){
        perror("\n Error en ftok\n");
        exit(-1);
    }

    //Paso 2
    shmid = shmget(llave, sizeof(char*), IPC_CREAT|0777);
    if(shmid == -1){
        perror("\n Error en shmget\n");
        exit(-1);
    }

    //Paso 3
    pila = (ArrCar *) shmat(shmid, 0, 0);
    if(pila == NULL){
       perror("\n Error en shmat\n");
       exit(-1);
    }

    // Incio del proceso
    while(1){
        printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        printf("\n 1 Ingresar elemento.\n 2 Retirar elemento.\n 3 Visualizar elemento.\n 4 Visualizar pila.\n 5 Finalizar\n\n Opción >\t");
        scanf("%i", &opcion);

        switch (opcion) {
            case 1:
                printf(" Ingresa un caracter: ");
                fflush(stdin);
                scanf("%s", in);
                push(*pila, in);
                break;
            case 2:
                pop(*pila);
                break;
            case 3:
                printf(" El elemento es: %c", top(*pila));
                break;
            case 4:
                printf(" Toda la pila es %s", *pila);
                break;
            case 5:
                // Paso 4
                // 4-a
                shmdt((ArrCar *)pila);
                // 4-b
                shmctl(shmid, IPC_RMID, 0);
                exit(0); // Terminamos el programa
                break;
            default:
                printf("\n Opcion invalida.\n");
                break;
        }
    }

    return 0;
}