/* 
    Autor: Carlos Nevárez
    Sistemas Operativos - 20/Marzo/2020
    Manda a llamar a dos funciones y cada una manda un print a la pantalla. Para simple observacion de funcionamiento.    
*/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h> //Para sleep

void funcion_a(){
    while(1){
        printf("\n Soy la funcion A\n");
        sleep(1);
    }
}

void funcion_b(){
    while(1){
        printf("\n Soy la funcion B\n");
        sleep(1);
    }
}

int main(){

    funcion_a();
    funcion_b(); // Nunca va pasar a esta funcion, porque la funcion A se queda imprimiendo. Estos programas son SECUENCIALES.

    return 0;
}