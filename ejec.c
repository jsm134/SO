#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
//modulo para despertar a un proceso
void despertar(){
}
//lamada a ls
void ls(){
	int pid;
	pid = fork();
	if(pid != 0){	//si es padre
		wait(NULL);
	}
	else{
		//llamada a ls desde la consola
		execlp("ls", "ls", NULL);
	}
}
//modulo que llama a pstree
void pstree(){
	int pid;
	pid = fork();
	if(pid != 0){	//si es padre
		wait(NULL);
	}
	else{
		//llamada a pstree desde la consola
		execlp("pstree", "pstree", NULL);
	}
}

int main(int argc, char *argv[]){
	int i, pid, pidejec, pidA, pidB, pidX, pidY;
	int tiempo;
	//si el ejecutable no tiene 3 argumentos (ejec _ _) no se inicia
	if(argc==3){
		//convertimos la variable tiempo 
		tiempo = atoi(argv[2]);
		printf("Soy el proceso ejec: mi pid es %d\n", getpid());
		//guardar el pid del proceso principal
		pidejec = getpid();	
		//se crea un nuevo proceso
		pid = fork(); 
		if(pid != 0){	//padre
			wait(NULL);	//padre espera a que su hijo muera (todos los hijos en realidad)
			printf("Soy ejec(%d) y muero\n", getpid());
		}
		else{
			//se le da a pidA el valor del proceso creado
			pidA = getpid();		
			printf("Soy el proceso A: mi pid es %d. Mi padre %d\n",pidA,pidejec);
			//creacion de proceso
			pid = fork(); //pidB
			if(pid != 0){	//si es padre
				//señal(n_senyal, funcion)
				signal(SIGUSR1, despertar);
				signal(SIGUSR2, pstree);
				//esperar a que el hijo muera											
				wait(NULL);
				printf("Soy A(%d) y muero\n", pidA);
			}
			else{	//si es hijo
				//se le da a pidB el valor del proceso creado anteriormente
				pidB = getpid();
				printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n", pidB, pidA, pidejec); 
				//3 porque hay que hacer 3 procesos
				for(i = 1; i <= 3; i++){
					//creacion de un proceso
					pid = fork();
					if(pid != 0){	//si es padre
						if(i==1)
							//se asigna pid a pidX
							pidX = pid;
						if(i==2)
							//se asigna pid a pidY
							pidY = pid;
					}
					if(pid == 0){	//Si es hijo				
						if(i==1){
							printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mis bisabuelo es %d\n", getpid(), pidB, pidA, pidejec);
							//si recibe sigsr1, se despierta el proceso
							signal(SIGUSR1, despertar);
							//si recibe sigsr2, se ejecuta ls
							signal(SIGUSR2, ls);
							//con pause() el proceso se espera hasta que llegue la señal
							pause();
							//si recibe sigalarm se despierta el proceso
							signal(SIGALRM, despertar);
							//tiempo que debe esperar para enviar sigalarm
							alarm(3);
							//el proceso se espera hasta que llegue la señal
							pause();
							printf("Soy X(%d) y muero\n", getpid());					
						}			
						else if(i==2){
							printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mis bisabuelo es %d\n", getpid(), pidB, pidA, pidejec);
							//si recibe sigsr1, se despierta el proceso
							signal(SIGUSR1, despertar);
							//si recibe sigsr2, se ejecuta ls
							signal(SIGUSR2, ls);
							//con pause() el proceso se espera hasta que llegue la señal
							pause();
							//si recibe sigalarm se despierta el proceso
							signal(SIGALRM, despertar);
							//tiempo que debe esperar para enviar sigalarm
							alarm(2);
							//el proceso se espera hasta que llegue la señal
							pause();							
							printf("Soy Y(%d) y muero\n", getpid());
						}
						else if(i==3){
							printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mis bisabuelo es %d\n", getpid(), pidB, pidA, pidejec);
							//si recibe sigalarm se despierta el proceso
							signal(SIGALRM, despertar);
							//tiempo que debe esperar para enviar sigalarm
							alarm(tiempo);
							//el proceso se espera hasta que llegue la señal
							pause();
							printf("me despierto y mando señales\n");
							if(argv[1][0]=='A'){
								//llama a pidA y le pasa la señal SIGUSR2 (pstree)
								kill(pidA, SIGUSR2);
								//llama a pidX y le pasa la señal SIGUSR1 (despertar)
								kill(pidX, SIGUSR1);
								//llama a pidY y le pasa la señal SIGUSR1 (despertar)
								kill(pidY, SIGUSR1);
							}else if (argv[1][0]=='B'){
								//llama a pidB y le pasa la señal SIGUSR2 (pstree)
								kill(pidB, SIGUSR2);
								//llama a pidX y le pasa la señal SIGUSR1 (despertar)
								kill(pidX, SIGUSR1);
								//llama a pidY y le pasa la señal SIGUSR1 (despertar)
								kill(pidY, SIGUSR1);
							}else if(argv[1][0]=='X'){
								//llama a pidX y le pasa la señal SIGUSR2 (ls)
								kill(pidX, SIGUSR2);
								//llama a pidY y le pasa la señal SIGUSR1 (despertar)
								kill(pidY, SIGUSR1);
							}else if(argv[1][0]){
								//llama a pidX y le pasa la señal SIGUSR1 (despertar)
								kill(pidX, SIGUSR1);
								//llama a pidY y le pasa la señal SIGUSR2 (ls)
								kill(pidY, SIGUSR2);
							}
							//espera recibir SIGALARM
							signal(SIGALRM, despertar);
							//espera 1 segundo para mandar SIGALARM
							alarm(1);
							//el proceso espera hasta que le llegue una señal
							pause();			
							printf("Soy Z(%d) y muero\n", getpid());
						}
					break;
					}	
				}
				//si todos los procesos se han ejecutado correctamente
				if(i == 4){
					//espera recepcion de señal SIGURS1 para despertar el proceso B
					signal(SIGUSR1, despertar);	
					//espera recepcion de señal SIGURS2 para ejecutar pstree					
					signal(SIGUSR2, pstree);
					//esperar hasta que los 3 hijos mueran	
					for(i = 1; i <= 3; i++){
						wait(NULL);
					}
					printf("Soy B(%d) y muero\n", getpid());
				}
			}
		}
	}
}