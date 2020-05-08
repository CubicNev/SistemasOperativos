# include <stdio.h>
# include <unistd.h>

int main(){
    printf("\n Duplicaremos la salida estandar en %i\n", dup(1));
    write(3,"Buenos dias\n",13);

    //Asigna los primeros descriptores de archivos disponibles a la dupliacion
    close(0);
    printf("\n Duplicaremos la salida estandar en %i\n", dup(1));

 return 0;
}