# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>

int main(){
    int pid;
    int fd[2]; // Tuberia

    if(pipe(fd) == -1){
        perror("\n Error en pipe.\n");
        exit(-1);
    }

    pid = fork();

    if(pid==-1){
        perror("\n Error en fork.\n");
        exit(-1);
    } else if(pid == 0){ //Hijo
        close(0); // Cierra STDIN del proceso hijo
        dup(fd[0]); // Coloca un fd de lectura en el espacio cerrado

        /* Redireccion */
        close(1); // Cierra STDOUT del proceso hijo
        open("archivo.txt", O_CREAT|O_RDWR|O_APPEND, S_IRWXU); // Abrimos o creamos (si no existe), el archivo.txt en lectura y escritura en el fd 1

        // Cierra las tuberias
        close(fd[0]);
        close(fd[1]);

        execlp("sort","sort", "-r", NULL);
        perror("\n Error en exec sort\n");
    } else { // Padre
        close(1); // cierra STDOUT del proceso padre
        dup(fd[1]); // Coloca un fd de escritura en el espacio disponible

        // Cierra las tuberias
        close(fd[0]);
        close(fd[1]);

        execlp("ls","ls", NULL);
        perror("\n Error en exec ls\n");
    }

    return 0;
}