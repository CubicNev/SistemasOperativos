# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# define SUBC 32

int tieneTuberias(char* comandos);
int tieneRD(char* comandos);
int tieneRI(char* comandos);
int cuentaToken(char *com, int t);
void execBasico(char *com, int numToken);
void redirectD(char *com);
void redirectDD(char *com);
void redirectI(char *com);
void execPipes(char *com);
void hacerPipe(char *comando[], int numTuberias, int numComandos);

void simulacion(char *com, int numToken);
void simD(char* com);
void simDD(char* com);
void simI(char* com);

// Devuelve verdadero(1) si encuentra un '|' en comandos
int tieneTuberias(char* comandos){
    if(strchr(comandos, '|')!=NULL){
        return 1;
    } else {
        return 0;
    }
}
// Devuelve verdadero(1) si encuentra un '>' en comandos
int tieneRD(char* comandos){
    if(strchr(comandos, '>')!=NULL){
        return 1;
    } else {
        return 0;
    }
}
// Devuelve verdadero(1) si encuentra un '<' en comandos
int tieneRI(char* comandos){
    if(strchr(comandos, '<')!=NULL){
        return 1;
    } else {
        return 0;
    }
}
// Cuenta el numero de espacios que hay en com
int cuentaToken(char *com, int t){
    int i=0, token=0;
    for(i=0;i<t;i++){
        if(com[i]==' '){
            token++;
        }
    }
    return token+1;
}

// en C0 | C1
// Conecta la salida C0 a la entrada de C2
void execPipes(char *comando){
    /* Para la ejecucion del comando */
    int pid, pidw;
    int fd[2];
    /* Para manejo de cadena: Se parte en dos, tomando la primera tuberia de la izquierda */
    int countCom, countPipe, countSubcad;
    char *subcad; // Para guardar temporalmente una subcadena
    int posPipe[SUBC];
    // iteraciones
    int i, j, k;


    //Recorro todo el comando entrante, y cuento el número de pipes y e número de subcadenas que se van a generar a partir de espcios.
    for(i=0, countPipe=0, countSubcad=0; i<strlen(comando); i++){
        if(comando[i] == '|'){ countPipe++; }
        if(comando[i] == ' '){ countSubcad++; }
    }
    countSubcad++; // No cuenta la última subcadena porque no tiene un espacio correspondiente (en un caso ideal)
    //printf("\n El comando %s tiene %i pipes y salen %i subcadenas.\n", comando, countPipe, countSubcad);

    /* Divide el comando en subcadenas cada que aparezca un espacio */
    char *subcadenas[countSubcad+1];
    subcad = strtok(comando, " ");
    i=0; j=0;
    while (subcad != NULL) {
        subcadenas[i] = subcad;
        subcad = strtok(NULL, " ");
        if(!strcmp(subcadenas[i], "|")) { // SI encuentra una tuberia
            posPipe[j] = i;//guarda su posicion 
            //printf(" Valor de posPipe %i = %i", j, i);
            j++;
        }
        printf("\n Subcad %i - %s\n", i, subcadenas[i]);
        i++;
    }

    /* Concatenamos las subcadenas para tener subcomandos */
    char subcomando0[SUBC] = "";
    char subcomando1[SUBC] = "";
    // Inicializa los subcomandos para su concatenacion 
    /*for(i=0; i<2; i++){
        subcomandos[i] = (char *) malloc(SUBC); // Los subcomandos tendran un tamaño maximo del valor de SUBC
    }*/

    //printf("\n Busca en la poscion %i.\n", posPipe[0]);
    for(i=0; i<countSubcad; i++){
        //printf("\n %s \n", subcadenas[i]);
        if(i < posPipe[0]) 
        {
            strcat(subcomando0, subcadenas[i]);
            if((i+1) != posPipe[0] )
                strcat(subcomando0, " ");
        } 
        else if(i > posPipe[0]) 
        {
            strcat(subcomando1, subcadenas[i]);
            //printf("\n %i Subcomando: %s\n", i, subcomando1);
            if(i != (countSubcad-1) )
                strcat(subcomando1, " "); // Les pone un espacio para separar los comandos que van llegando, si es que no les sigue un pipe y aun no esta a punto de acabar
        } else 
        {
            printf("\n Subcomando 0: %s.\n", subcomando0);
            continue;
        }

    }
    printf("\n Subcomando 1: %s.\n", subcomando1);

    /* 
        Inicia pre-ejecución: Como ya tenemos los distintos procesos que se van a ejecutar
            se tienen que ejecutar en orden, este sigue el comando de izquierda a derecha.
    */
    //int f = open("bitacora.txt", O_CREAT|O_WRONLY, S_IRWXU);
    
    // Incializacion de la tuberia sin nombre 
    if(pipe(fd) == -1){
        perror("\n Error en pipe.\n");
        exit(-1);
    }
    pid = fork();

    if(pid==-1)
    {
        perror("\n Error en fork\n");
		exit(EXIT_FAILURE);
    } 
    else 
    {
        if(pid==0) // Hijo
        {
            /*close(fd[0]); // Cerramos extremo de lectura
            dup2(fd[1], STDOUT_FILENO); // El extremo de escritura se duplica en STDOUT
            close(fd[1]); // Cerramos extremo de escritura
            */
            //execBasico(subcomandos[0], cuentaToken(subcomandos[0], strlen(subcomandos[0])));
            simulacion(subcomando0, cuentaToken(subcomando0, strlen(subcomando0)));
        } 
        else // padre
        {
            /*close(fd[1]); // Cerramos extremo de escritura
            dup2(fd[0], STDIN_FILENO); // El extremo de lectura se duplica en STDIN
            close(fd[0]); // Cerramos extremo de lectura*/
            pidw = wait(NULL);
            //execBasico(subcomandos[1], cuentaToken(subcomandos[1], strlen(subcomandos[1])));
            simulacion(subcomando1, cuentaToken(subcomando1, strlen(subcomando1)));
        }
    }
    
}

void simulacion(char *com, int numToken){
    if(strchr(com, '|')!=NULL){
        execPipes(com);
    } else {
        if(strchr(com, '>')!=NULL){
            if(strstr(com, ">>")!=NULL){
                simDD(com);
            }else{
                simD(com);
            }
        }else{
            if(strchr(com, '<')!=NULL){
                simI(com);
            }else {
                char delim[] = " ";
                char *token[numToken];
                int i;
                char *ptr = strtok(com, delim);
                i=0;
                while(ptr != NULL){
                    token[i] = ptr;
                    ptr = strtok(NULL, delim);
                    i++;	
                }
                token[i] = NULL;
                printf("\n Ejecutar %s.\n", token[0]);
                exit(0);
            }
        }
    }
}

// Cambia la salida del proceso
void simD(char *com){ 
    // Cierra el fd 1 (STDOUT)
    //close(STDOUT_FILENO);
    /*Divide el comando en dos tokenPipe0 > tokenPipe1 (será un archivo)*/
    char *tokenPipe[2];
    int i;
    char *ptr = strtok(com, ">");
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr;
		ptr = strtok(NULL, ">"); // EL primer argumento es NULL porque la funcion recuerda la palabra
		i++;	
	}
    tokenPipe[i] = NULL;
    /*Quita el espacio del tokenPipe[1]*/
    char *com2 = tokenPipe[1];
    char *nombre[2];
    char *ptr2 = strtok(com2, " ");// Quita el espacio
	i=0; // Se guardara en nombre[0]
	while(ptr2 != NULL){
		nombre[i] = ptr2;
		ptr2 = strtok(NULL, " ");
		i++;	
	}
    nombre[i] = NULL;
    /* Redirecciona */
    int numToken;
    int t;
    // Abre el archivo de tokenPipe1, ya sin el espacio. Estara en el descriptor de archivos 1 del proceso
    //open(nombre[0], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU); // se abre en modo de escritura
    printf("\n Redireccion > %s\n", nombre[0]);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    // Ejecuta un exec con los comandos en tokenPipe0
    simulacion(tokenPipe[0], numToken);
    
}
// Cambia la salida el proceso en modo APPEND
void simDD(char *com){  
    /* Cierra el fd 1 (STDOUT) */
    //close(STDOUT_FILENO);
    /* Divide el comando en dos tokenPipe0 > tokenPipe1 (será un archivo) */
    char *tokenPipe[2];
    int i;
    char *ptr = strtok(com, ">>");
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr;
		ptr = strtok(NULL, ">>");
		i++;	
	}
    tokenPipe[i] = NULL;
    /*Quita el espacio del tokenPipe[1]*/
    char *com2 = tokenPipe[1];
    int numToken;
    int t;
    char *nombre[2];
    char *ptr2 = strtok(com2, " ");
	i=0;
	while(ptr2 != NULL){
		nombre[i] = ptr2;
		ptr2 = strtok(NULL, " ");
		i++;	
	}
    nombre[i] = NULL;
    /* Redirecciona en modo APPEND */
    //open(nombre[0], O_CREAT|O_RDWR|O_APPEND, S_IRWXU); // Ahora el fd 1 del proceso tiene al archivo de tokenPipe0
    printf("\n Redireccion >> %s\n", nombre[0]);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    /* Ejecuta un exec con los comandos en tokenPipe0 */
    simulacion(tokenPipe[0], numToken);
}
// Cambia la entrada del proceso
void simI(char *com){ 
    /* Cierra el fd 0 (STDOUT) */
    //close(STDIN_FILENO);
    /* Divide el comando en dos tokenPipe0 > tokenPipe1 (será un archivo) */
    char *tokenPipe[2];
    int i;
    char *ptr = strtok(com, "<");
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr;
		ptr = strtok(NULL, "<");
		i++;	
	}
    tokenPipe[i] = NULL;
    /*Quita el espacio del tokenPipe[1]*/
    char *com2 = tokenPipe[1];
    int numToken;
    int t;
    char *nombre[2];
    char *ptr2 = strtok(com2, " ");
	i=0;
	while(ptr2 != NULL){
		nombre[i] = ptr2;
		ptr2 = strtok(NULL, " ");
		i++;	
	}
    nombre[i] = NULL;
    /*Abre el archivo en tokenPipe0 (ya sin el espacio) en el FD 0*/
    //open(nombre[0], O_RDONLY,  S_IRWXU);
    printf("\n Redireccion < %s\n", nombre[0]);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    simulacion(tokenPipe[0], numToken);
}