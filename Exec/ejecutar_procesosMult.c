# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

int main() {

    int pid;
    pid = fork();

    if(pid == -1) {
        perror("\nError en fork\n");
        exit(-1);
    } else if(pid == 0){
        printf("\n---------------------------------- \n");
        printf("\nEjecutaré ./suma mediante exec  \n");
        printf("\n---------------------------------- \n");

        /* execl tambien es para ordianrios, y su sintaxis es execl(char *path, char *arg0, ..., char *argN, (char *)0) */
        execl("/bin/ls", "ls", "-l", "-a", NULL);

        perror("\nError en exec\n");
        printf("\nADIOS\n");
    } else {
        wait(NULL);
        printf("\n Mi hijo termino de ejecutarse \n");
    }
    

 return 0;
}