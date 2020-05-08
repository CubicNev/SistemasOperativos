# include <stdio.h>
# include <unistd.h>

int main() {
    //char *argumentos[] = {"./suma", "4.6", "6.9", NULL};

    printf("\n---------------------------------- \n");
    printf("\nEjecutaré ./suma mediante exec  \n");
    printf("\n---------------------------------- \n");

    /* execv es para programas ordinarios, su sintaxis es int execv(char *path, char *argv[])*/
    //execv(argumentos[0], argumentos);
    /* execl tambien es para ordianrios, y su sintaxis es execl(char *path, char *arg0, ..., char *argN, (char *)0) */
    execl("/bin/ls", "ls", "-l", "-a", NULL);

    perror("\nError en exec\n");
    printf("\nADIOS\n");

 return 0;
}