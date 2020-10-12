/*
    Sistemas Operativos
    Autores: Carlos Nevarez, Uriel Cabrera
*/
# include <limits.h>  
# include "fshell.h"
# define T_ARGS 128

int main(int argc, char *argv[]){
    int pid, pidw;
    int cadEsSalir, i;
    char comandos[T_ARGS];
    char *argVect[T_ARGS];
    char delim[] = " ";
    char *palabraDividida;
    char usr[LOGIN_NAME_MAX]; 
    char eqp[HOST_NAME_MAX];
    char dir[PATH_MAX];

    while(1){
        getlogin_r(usr, LOGIN_NAME_MAX);
        gethostname(eqp, HOST_NAME_MAX);
        getcwd(dir, PATH_MAX);

        printf(" %s&%s:- %s > ", usr, eqp, dir);
        fgets(comandos, sizeof(comandos), stdin);

        comandos[strlen(comandos)-1] = '\0'; 
        cadEsSalir = strcmp(comandos, "exit");

        if(cadEsSalir != 0){
            int t = strlen(comandos);
            pid = fork();

            if(pid == -1){
                perror("\n Error en fork\n");
				exit(EXIT_FAILURE);
            } else if(pid == 0){
                int numToken = cuentaToken(comandos,t);
                execBasico(comandos, numToken);
                printf("\n");
				exit(1);
            } else {
                pidw = wait(NULL);
            }
        } else {
            break;
        }  
    }
    return 0;
}
