# include <stdio.h>
# include <stdlib.h>
# include <math.h>

int main(int argc, char *argv[]){
    float n1, n2, suma;

    if(argc != 3){
        printf("\n Introduce los dos números a sumar\n");
        exit(-1);
    }

    n1 = atof(argv[1]);
    n2 = atof(argv[2]);
    suma = n1 + n2;

    printf("\n++++++++++++++++++++++++++++++++++++++++\n");
    printf("\nSoy el programa %s, me invocó exec\n", argv[0]);
    printf("\nLa suma de %f y de %f es %f\n", n1, n2, suma);
    printf("\n++++++++++++++++++++++++++++++++++++++++\n");
}