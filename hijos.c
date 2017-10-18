#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char * argv[]){
	int x, y, pid, i, j;
	int shmidx, shmidy;
	int *pidx, *pidy;
	if(argc == 3){
		x = atoi(argv[1]);
		y = atoi(argv[2]);
		//Asigno el espacio
		shmidx = shmget(IPC_PRIVATE, sizeof(int) * (x + 1), IPC_CREAT | 0666);
		shmidy = shmget(IPC_PRIVATE, sizeof(int) * y, IPC_CREAT | 0666);
		// asigno al puntero la cantidad de memoria anterior
		pidx = (int *) shmat (shmidx, 0, 0);
		pidy = (int *) shmat (shmidy, 0, 0);
		pidx[0] = getpid();
		for(i = 1; i <= x; i++){
			//creacion de un proceso nuevo
			pid = fork();
			if(pid != 0){	//si no es hijo (es que es padre)
				//el padre espera a sus hijos antes de morir el
				wait(NULL);
				if(i == 1){
					printf("Soy el super padre (%d): mis hijos finales son:  ", getpid());
					//Se escriben por pantalla los procesos en vertical(x)
					for(j = 1; j <= x; j++){
						printf("%d ", pidx[j]);
					}
					//Se escriben por pantalla los procesos en horizontal(y)
					for(j = 0; j < y; j++){
						printf("%d ", pidy[j]);
					}
					printf("\n");
				}
				break;
			}
			else{
				//en caso de que nada de lo anterior sirva, únicamente añade un pid a pidx[i]
				pidx[i] = getpid();
				printf("Soy el subhijo %d, mis padres son: ", pidx[i]);
				//se escriben todos los procesos padres (x)
				for(j = 0; j < i; j++){
					printf("%d ", pidx[j]);
				}
				printf("\n");
			}
		}
		//si se ha conseguido crear correctamente todos los procesos anteriores
		if(i == x + 1){
			//creacion de procesos en horizontal (y)
			for(j = 0; j < y; j++){
				//crear proceso nuevo
				pid = fork();
				//si es hijo
				if(pid == 0){
					//añadir a pidy[j] el nuevo pid
					pidy[j] = getpid();
					//esperar 10 segundos para observar el proceso de los hijos
					sleep(10);
					break;
				}
			}
		}
		//esperar 10 segundos y observar que solo queda el superpadre
		sleep(10);
	}
	return 0;
}