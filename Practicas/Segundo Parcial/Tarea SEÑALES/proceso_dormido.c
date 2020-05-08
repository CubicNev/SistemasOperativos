# include <stdio.h>
# include <unistd.h>
# include <signal.h>

void funcion_sigint(int sig){
	printf("\n Señal recibida %i\n", sig);
	printf("\n No moriré!!\n");
	/*
	No morira si presionas ctrl + c o si mandas
		$ kill -SIGINT pid
	desde otra terminal
	Cuando recibas SIGINT ejecuta estos prints uwu
	*/
}

int main() {
	//signal(SIGINT, SIG_IGN);
	signal(SIGINT, funcion_sigint);
	//kill(-1,SIGKILL); -Mata a la compu, cuidado xd. Checha $ man 2 kill
	while(1){
		pause(); // Espera una señal, en este caso ctrl + C. Checa $ man 2 kill
		printf("\nzzzzzzz.. (-_-)\n");
		printf("\nUwU\n");
		sleep(1);
	}
	
 return 1;
}
