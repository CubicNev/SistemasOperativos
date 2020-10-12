/*
    Sistemas Operativos
    Autores: Carlos Nevárez, Uriel Cabrera
*/
# include <limits.h>  // Libreria para que las cadenas con info. sobre la maquina tengan limites validos.
# include "pruebaShell.h"
# define T_ARGS 128

int main(int argc, char *argv[]){
    /* Variables para hacer fork */
    int pid, pidw;
    /* Variables para controlar el flujo del programa */
    int cadEsSalir, i;
    /* Variables para manejar los comandos */
    char comandos[T_ARGS];
    char *argVect[T_ARGS]; // Vector para hacer exec
    char delim[] = " "; // Delimitador para dividir la entrada en comandos, cada espacio es una palabra
    char *palabraDividida;
    /* Variables para la informacion del prompt */
    char usr[LOGIN_NAME_MAX]; // Usuario
    char eqp[HOST_NAME_MAX]; // Equipo
    char dir[PATH_MAX]; // Directorio

    while(1){
        /* Obtencion de informacion para el prompt */
        getlogin_r(usr, LOGIN_NAME_MAX);
        gethostname(eqp, HOST_NAME_MAX);
        getcwd(dir, PATH_MAX);

        /* Despliegue del prompt */
        printf(" %s&%s:- %s > ", usr, eqp, dir);
        fgets(comandos, sizeof(comandos), stdin);

        /* Manejo de los comandos */
        comandos[strlen(comandos)-1] = '\0'; // Quita el salto de linea que trae el fgets
        cadEsSalir = strcmp(comandos, "exit");

        if(cadEsSalir != 0){ // Si la cadena de comandos no es "exit"

            /* Manejo de los comandos que se estan mandando al exec / PROCESO PRINCIPAL*/
            /* palabraDividida = strtok(comandos, delim); // Divide la entrada de comandos, segun cada espacio
            i=0;
            while(palabraDividida != NULL){
                argVect[i] = palabraDividida;
                palabraDividida = strtok(NULL, delim);
                i++;
            }
            argVect[i] = NULL; */
            int t = strlen(comandos);
            /* Ejecucion */
            pid = fork();

            if(pid == -1){
                perror("\n Error en fork\n");
				exit(EXIT_FAILURE);
            } else if(pid == 0){ // Codigo del hijo / EJECUCION DE COMANDOS
                /*execv(argVect[0], argVect);
				perror("\n Error en excec \n"); */
                int numToken = cuentaToken(comandos,t);
                simulacion(comandos, numToken);
                printf("\n");
				exit(1);
            } else { // Codigo del padre
                pidw = wait(NULL);
            }

        } else { // Fin del programa
            //printf(" Adios\n");
            break;
        }  
    }
    return 0;
}
