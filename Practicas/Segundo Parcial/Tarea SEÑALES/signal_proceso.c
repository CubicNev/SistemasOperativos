# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>

int main() {
 int pid;
	pid=fork();
	if(pid==-1){
		perror("Error en fork");
		exit(-1);	
	} else if (pid==0){
		while(1){
			printf("\nzzzzzzz.. (-_-)\n");
			sleep(1);
		}	
	} else {
		sleep(5);
		printf("\n Soy el padre de %i y lo maté xD\n", pid);
		kill(pid, SIGTERM);
	}
 return 1;
}
