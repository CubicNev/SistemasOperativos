/**
 * Sistemas Operativos.
 * Autor: Carlos Nevárez
 * Descripcion: Este programa recibe un comando con pipes y lo divide en subcomandos: subomando0 | subcomando1 | subcomando2. Para esto primero descompone el comando por espacios.
 * Bug: 
 *  06/06/2020 - Metiendo el comando "ls -a | sort -n | wc > archivo.txt" le quita .txt
 *  Sol: agregar un poco mas de espacio de memoria cuando se crean los arreglos de subcadenas y subcomandos, al parecer con agregarles 1 a su tamaño basta.
 */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>

# define TAM 64
# define SUBC 16

int cuentaToken(char *com, int t);
void execBasico(char *com, int numToken);

void ejecucion(char *com){
    printf("\n Ejecucion de: %s\n", com);
}

int main(){
    /* Para la ejecucion del comando */
    int pid, pidw;
    int fd[2];
    /* Para manejo de cadena */
    char comando[TAM];
    int countCom, countPipe, countSubcad, i, j, k;
    char *subcad; // Para guardar temporalmente una subcadena

    /* Entrada de cadena */
    printf("\n Introduce un comando: ");
    fgets(comando, TAM, stdin);
    comando[strlen(comando)-1] = '\0'; // Quita el ENTER

    /* Cuento el número de pipes */
    for(i=0, countPipe=0, countSubcad=0; i<strlen(comando); i++){
        if(comando[i] == '|'){ countPipe++; }
        if(comando[i] == ' '){ countSubcad++; }
    }
    countSubcad++; // No cuenta la última subcadena porque no tiene un espacio correspondiente (en un caso ideal)
    printf("\n El comando %s tiene %i pipes y salen %i subcadenas.\n", comando, countPipe, countSubcad);

    /* Divide el comando en subcadenas cada que aparezca un espacio */
    char *subcadenas[countSubcad+1];
    subcad = strtok(comando, " ");
    i=0;
    j=0;
    while (subcad != NULL) {
        subcadenas[i] = subcad;
        subcad = strtok(NULL, " ");
        //printf("\n Subcad %i - %s\n", i, subcadenas[i]);
        if(!strcmp(subcadenas[i], "|")){
            j++;
        }
        i++;
    }

    /* Concatenamos las subcadenas para tener subcomandos */
    countCom = countPipe + 2;
    char *subcomandos[countCom];
    // Inicializa los subcomandos para su concatenacion 
    for(i=0; i<countCom; i++){
        subcomandos[i] = (char *) malloc(SUBC); // Los subcomandos tendran un tamaño maximo de 16
    }

    for(i=0, j=0; i<countSubcad; i++){

        if(!strcmp(subcadenas[i], "|")){
            printf("\n Subcomando %i: %s.\n", j, subcomandos[j]);
            j++;
        } else {
            //printf("\n %s %i Concatena %s\n", subcomandos[j], j, subcadenas[i]);
            strcat(subcomandos[j], subcadenas[i]); // Concatenacion
            if(strcmp(subcadenas[i+1], "|") && i!=(countSubcad-1) )
                strcat(subcomandos[j], " "); // Les pone un espacio para separar los comandos que van llegando, si es que no les sigue un pipe y aun no esta a punto de acabar
        }

    }
    printf("\n Subcomando %i: %s.\n", j, subcomandos[j]);
    subcomandos[j+1] = NULL;


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

    for(i=0, j=0; i<countPipe; i++){

        // Llamada a fork, se ejecutaran 2 subcomandos, estos comparten tuberia. 
        pid = fork();

        if(pid == -1){
            perror("\n Error en fork.\n");
            exit(-1);
        } else if(pid == 0){ // Hijo
        
            close(0); // Cierra el FD correspondiente a STDIN
            dup(fd[0]); // Sustituye el FD cerrado por el lado de lectura de la tuberia
            
            // Cierre de tuberias
            close(fd[0]);
            close(fd[1]);
        
            // Ejecucion del comando
            
            break;
        } else { // Padre
        
            close(1); // Cierra el FD correspondiente a STDOUT
            dup(fd[1]); // Sustituye el FD cerrado por el lado de escritura de la tuberia

            // Cierre de tuberias 
            close(fd[0]);
            close(fd[1]);
        
            // Ejecucion del comando    
            
        }
        wait(NULL);
    }

    return 0;
}

void execPipes(char *comando[], int numTuberias, int numComandos){
    int status;
    int i = 0;
    int pid;
    int pipefds[2*numTuberias];

    for(i=0; i< numTuberias; i++){
        if(pipe(pipefds + i*2) < 0 ){
            perror("\n Error en pipe.\n");
            exit(-1);
        }
    }

    int j = 0;
    int i = 0;
    int k;
    while(comando[i] != NULL){
        pid = fork();
        if(pid == -1){
            perror("\n Error en fork.\n");
            exit(-1);
        } else if(pid == 0){
            // SI no es el ultimo comando
            if(comando[i+1] != NULL){
                if(dup2(pipefds[j+1], 1) < 0){
                    perror("\n Error en dup.\n");
                    exit(EXIT_FAILURE);
                }
            }
            // SI no es el primer comando && j != 2*numTuberias
            if(j != 0) {
                if(dup2(pipefds[j-2], 0) < 0){
                    perror("\n Error en dup.\n");
                    exit(EXIT_FAILURE);
                }
            }
            for(k=0; k<2*numTuberias; k++){
                close(pipefds[k]);
            }
            execBasico(comando[i], cuentaToken(comando[i], strlen(comando[i])));
            i++;
            j += 2;
        }
    }

    /* Cerrar todas las tuberias y esperar por los hijo */
    for(i=0; i<2*numTuberias; i++){
        close(pipefds[i]);
    }
    for(i=0; i<numTuberias+1; i++){
        wait(&status);
    }
}


// conecta la entrada de un proceso a la salida de otro
void execPipes(char *com){
    // Divide el comando hasta en 3 comandos
    char *tokenPipe[3]; // Arreglo de comandos
    int i;
    char *ptr = strtok(com, "|"); // Divide en subcadenas cada que encuentre un |
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr; // Guarda la subcadena en el arreglo de comandos
		ptr = strtok(NULL, "|");
		i++;	
	}
    tokenPipe[i] = NULL;


    int numToken;
    int t;
    int numpipes=i-1;
    if(numpipes==1){
        int fd[2];
        int pid;
        int status;
        pipe(fd);
        pid = fork(); // 0

        if(pid==-1){
            perror("Fallo la ejecucion UnU\n");
        }else if(pid==0){ 
            close(fd[0]); //Cerramos extremo de lectura
            dup2(fd[1], STDOUT_FILENO); // El extremo de escritura se duplica en STDOUT
            close(fd[1]); //Cerramos extremo de escritura

            t = strlen(tokenPipe[0]);
            numToken = cuentaToken(tokenPipe[0], t);
            execBasico(tokenPipe[0], numToken);
        } else {
            close(fd[1]);
            pid = fork(); // 1
            if(pid==-1){
                perror("Fallo la ejecucion UnU\n");
            }else{ // PADRE
                if(pid==0){ // Hijo
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]); //Cerramos extremo de lectura

                    t = strlen(tokenPipe[1]);
                    numToken = cuentaToken(tokenPipe[1], t);
                    execBasico(tokenPipe[1], numToken);
                }else{  //Padre
                    close(fd[0]);
                }
            }
        }
        
        wait(&status);
        wait(&status);
    }
    if(numpipes==2){
        int fd[2], fd2[2];
        int pid;
        int status;

        pipe(fd);
        pipe(fd2);
        pid = fork(); // 0

        if(pid==-1){
            perror("Fallo la ejecucion UnU\n");
        }else if(pid==0){ // HIJO
            close(fd[0]); //Cerramos extremo de lectura
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]); //Cerramos extremo de escritura

            t = strlen(tokenPipe[0]);
            numToken = cuentaToken(tokenPipe[0], t);
            execBasico(tokenPipe[0], numToken);
        } else { // PADRE
            close(fd[1]);
            pid = fork();  // 1
            if(pid==-1){
                perror("Fallo la ejecucion UnU\n");
            }else if(pid==0){ //HIJO
                close(fd2[0]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]); //Cerramos extremo de lectura
                dup2(fd2[1], STDOUT_FILENO);
                close(fd2[1]);

                t = strlen(tokenPipe[1]);
                numToken = cuentaToken(tokenPipe[1], t);
                execBasico(tokenPipe[1], numToken);
            } else { // PADRE
                close(fd[0]);
                close(fd2[1]);

                pid = fork(); // 2
                if(pid==-1){
                    perror("Fallo la ejecucion UnU\n");
                }else{ //HIJO
                    if(pid==0){ // Hijo de 3
                        close(fd2[1]);
                        dup2(fd2[0], STDIN_FILENO);
                        close(fd2[0]);

                        t = strlen(tokenPipe[2]);
                        numToken = cuentaToken(tokenPipe[2], t);
                        execBasico(tokenPipe[2], numToken);
                    }
                    else{ // PADRE
                        close(fd2[0]);
                    }
                }
            }
            
        }
        
        wait(&status);
        wait(&status);
        wait(&status);
    }
}
