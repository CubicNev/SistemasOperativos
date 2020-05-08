# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# define T_MENSAJE 200

int main(){
    int pid, adios;
    int filedes[2];
    char mensaje[T_MENSAJE];

    if(pipe(filedes)==-1){
        perror("\n Error en pipe\n");
        exit(-1);
    }

    printf("\n %i, %i\n", filedes[0], filedes[1]);
    pid = fork();

    if(pid==-1){
        printf("\n Error en fork\n");
        exit(-1);
    } else if(pid==0){ //hijo
        adios = 1;
        while(adios!=0){
            read(filedes[0], mensaje, sizeof(mensaje)); //Leer mensaje
            printf("\n HIJO > mensaje leído: %s\n", mensaje);
            adios = strcmp(mensaje,"ADIOS\n");
        }
        exit(0);
    } else { //padre
        adios = 1;
        while(adios != 0){
            printf("\n PADRE > Introduce mensaje: ");
            fgets(mensaje, sizeof(mensaje), stdin);
            write(filedes[1], mensaje, strlen(mensaje)+1); //Escribir en la tubería
            adios = strcmp(mensaje, "ADIOS\n");
            sleep(1);
        }
        wait(NULL);
    }

 return 0;
}