/*
Autor: Carlos Nevárez
Fecha: 22-04-2020
Programa donde se tienen dos procesos:
 El proceso 1 escribe mensajes de tipo 1, y va a leer solo mensajes de tipo 2.
 El proceso 2 lee mensajes de tipo 1, a su vez tambien va a escribir mensajes pero de tipo 2.
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

// Definicion del mensaje
struct mensaje{
    long tipo;
    char cadena[50];    
};

int main(){
    //Variable para enviar y rebir el mensaje
    struct mensaje msg;
    //Llave para el paso 1
    key_t llave;
    //Identificador para la cola de mensajes del paso 2
    int msqid;
    //Para calcular el tamaño del mensaje
    int msqsize = sizeof(msg) - sizeof(msg.tipo);
    // Para capturar errores al enviar
    int snd;
    // Pra capturara errores al recibir
    int rcv;

    // Paso 1: Obtener la llave
    llave = ftok("/bin/pwd", 7);
    if(llave==-1){
        perror("\n Error en ftok\n");
        exit(-1);
    }

    // Paso 2: Crear la cola de mensajes
    msqid = msgget(llave, IPC_CREAT|0777);
    if(msqid==-1){
        perror("\n Error en msgget\n");
        exit(-1);
    }

    // Paso 3: Manipular la cola de mensajes
    /* Prepara el mensaje */
    msg.tipo = 2;
    printf("\n Introduce un mensaje: ");
    fgets(msg.cadena, sizeof(msg.cadena), stdin);

    printf("\n Enviando mensaje . . .\n");
    snd = msgsnd(msqid, &msg, msqsize, 0); // colocar mensaje en la cola
    if(snd==-1){
        perror("\n Error en msgsnd\n");
        exit(-1);
    }

    printf("\n Esperando mensaje . . .\n");
    rcv = msgrcv(msqid, &msg, msqsize, 1, 0); // leer mensaje de la cola
    if(rcv==-1){
        perror("\n Error en msgrcv\n");
        exit(-1);
    }
    printf("\n Mensaje recibido:\n\tTipo: %li\n\tCadena: %s\n\n", msg.tipo, msg.cadena);

    // Paso 4: liberar la cola de mensajes
    msgctl(msqid, IPC_RMID, 0);

    return 0;
}