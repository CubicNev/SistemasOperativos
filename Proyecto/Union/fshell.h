# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

int tieneTuberias(char* comandos);
int tieneRD(char* comandos);
int tieneRI(char* comandos);
int cuentaToken(char *com, int t);
void execBasico(char *com, int numToken);
void redirectD(char *com);
void redirectDD(char *com);
void redirectI(char *com);
void execPipes(char *com);
char *quitarEspacio(char *nom);

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
// Si no hay redirecciones en com hace la ejecucion de comandos normal, si encuentra redirecciones manda al correspondiente
void execBasico(char *com, int numToken){
    if(strchr(com, '>')!=NULL){
        if(strstr(com, ">>")!=NULL){
            redirectDD(com);
        }else{
           redirectD(com);
        }
    }else{
        if(strchr(com, '<')!=NULL){
            redirectI(com);
        }else{
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
            execvp(token[0], token);
            perror("Error en la ejecución: No se ha podido ejecutar\n");
            exit(0);
        }
    }
    
}

void redirectD(char *com){
    close(STDOUT_FILENO);
    char *tokenPipe[2];
    int i;
    char *ptr = strtok(com, ">");
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr;
		ptr = strtok(NULL, ">");
		i++;	
	}
    tokenPipe[i] = NULL;
    int numToken;
    int t;
    tokenPipe[1] = quitarEspacio(tokenPipe[1]);
    open(tokenPipe[1], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    execBasico(tokenPipe[0], numToken);
    
}

void redirectDD(char *com){ 
    close(STDOUT_FILENO);
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
    int numToken;
    int t;
    tokenPipe[1] = quitarEspacio(tokenPipe[1]);
    open(tokenPipe[1], O_CREAT|O_RDWR|O_APPEND, S_IRWXU);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    execBasico(tokenPipe[0], numToken);
}

void redirectI(char *com){
    close(STDIN_FILENO);
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
    int numToken;
    int t;
    tokenPipe[1] = quitarEspacio(tokenPipe[1]);
    open(tokenPipe[1], O_RDONLY,  S_IRWXU);
    t = strlen(tokenPipe[0]);
    numToken = cuentaToken(tokenPipe[0], t);
    execBasico(tokenPipe[0], numToken);
}

void execPipes(char *com){
    char *tokenPipe[3];
    int i;
    char *ptr = strtok(com, "|");
	i=0;
	while(ptr != NULL){
		tokenPipe[i] = ptr;
		ptr = strtok(NULL, "|");
		i++;	
	}
    tokenPipe[i] = NULL;
    int numToken;
    int t;
    int numpipes=i-1;

    //Comienza ejecución de las pipes
    if(numpipes==1){
        int fd[2];
        int pid;
        int status;
        pipe(fd);
        pid = fork();

        if(pid==-1){
            perror("Fallo la ejecucion UnU\n");
        }else{
            if(pid==0){ 
                close(fd[0]); //Cerramos extremo de lectura
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]); //Cerramos extremo de escritura
                t = strlen(tokenPipe[0]);
                numToken = cuentaToken(tokenPipe[0], t);
                execBasico(tokenPipe[0], numToken);
            }else{
                close(fd[1]);
                pid = fork(); 
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
        pid = fork();

        if(pid==-1){
            perror("Fallo la ejecucion UnU\n");
        }else{
            if(pid==0){ // Hijo de 1 = 1.1
                close(fd[0]); //Cerramos extremo de lectura
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]); //Cerramos extremo de escritura
                t = strlen(tokenPipe[0]);
                numToken = cuentaToken(tokenPipe[0], t);
                execBasico(tokenPipe[0], numToken);
            }else{ // Padre 1
                close(fd[1]);
                pid = fork(); 
                if(pid==-1){
                    perror("Fallo la ejecucion UnU\n");
                }else{ // Hijo de 2
                    if(pid==0){
                        close(fd2[0]);
                        dup2(fd[0], STDIN_FILENO);
                        close(fd[0]); //Cerramos extremo de lectura
                        dup2(fd2[1], STDOUT_FILENO);
                        close(fd2[1]);
                        t = strlen(tokenPipe[1]);
                        numToken = cuentaToken(tokenPipe[1], t);
                        execBasico(tokenPipe[1], numToken);

                    }else{ // Padre 2
                        close(fd[0]);
                        close(fd2[1]);
                        pid = fork();
                        if(pid==-1){
                            perror("Fallo la ejecucion UnU\n");
                        }else{
                            if(pid==0){ // Hijo de 3
                                close(fd2[1]);
                                dup2(fd2[0], STDIN_FILENO);
                                close(fd2[0]);
                                t = strlen(tokenPipe[2]);
                                numToken = cuentaToken(tokenPipe[2], t);
                                execBasico(tokenPipe[2], numToken);
                            }
                            else{ // Padre 3
                                close(fd2[0]);
                            }
                        }
                    }
                }
            }
        }
        wait(&status);
        wait(&status);
        wait(&status);
    }
}

char *quitarEspacio(char *nom){
    char *sal;
    int i;
    for(i=0; nom[i+1]!='\0'; i++){
        sal[i] = nom[i+1];
    }
    sal[i] = '\0';
    return sal;
}