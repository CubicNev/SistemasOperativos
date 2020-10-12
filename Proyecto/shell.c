
#include"minishellCN.h"

int main(){

    while(1){
        char comando[100];
        printf("ELCHAMINN-PC@minishell:-$ ");
        scanf(" %99[^\n]", comando);
        int t = strlen(comando);
        if(!strcmp("exit", comando)){
            break;
        }
        int pipe;
        if(strchr(comando, '|')!=NULL){
            pipe=1;
        }else{
            pipe=0;
        }
        
        int pid;
        pid = fork();
        if(!pid){
            if(pipe==0){
                int numToken = cuentaToken(comando,t);
                execBasico(comando, numToken);
            }
            else{
                execPipes(comando);
            }
        }else{
            wait(NULL);
        }
    }

    return 0;
}

