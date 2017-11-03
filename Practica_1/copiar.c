#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 10
int main(int argc, char *argv[]){
	// creo una variable con el tamaño a guardar
	char buffer[SIZE];
	// inicializo varibale tuberia de dos valores (0 y 1)
	// tras hacer un pipe se podria leer y escribir con esta variable
	int tuberia[2], pid;
	int readbytes;
	if(argc == 3){
		int archivo_origen = open(argv[1], O_RDONLY);
		if(archivo_origen < 0) {
			printf("Error al abrir %s no se encuentra o falta contenido\n", argv[1]);
		}
		else{
			// se crea la tuberia
			// tuberia[0] - lectura
			// tuberia[1] - escritura
			pipe(tuberia);
			pid = fork();
			//Si el proceso es padre	
			if(pid != 0){ //proceso padre
				//cerrar lectura para que se pueda hacer bien la comunicacion entre el padre y el hijo
				close(tuberia[0]);
				//inicializar tamaño al de la primera linea
				readbytes = read(archivo_origen, buffer, SIZE);
				//mientras haya contenido en el domcumento
				while(readbytes != 0){
					//escribir en la tuberia
					write(tuberia[1], buffer, readbytes);	
					//actualizamos readbytes a la siguiente linea
					readbytes = read(archivo_origen, buffer, SIZE);						
				}
				//cerramos el archivo de origen.txt
				close(archivo_origen);
				//cerramos la tuberia de intercambio (escritura)
				close(tuberia[1]);
			}
			//si el proceso es hijo
			else{
				//cerrar tuberia de escritura para que se pueda hacer bien la comunicacion
				close(tuberia[1]);
				//crear el archivo de destino con lectura y escritura
				int archivo_destino = creat(argv[2], 0777);
				//inicializar tamaño a la primera linea
				readbytes = read(tuberia[0], buffer, SIZE);
				//mientras haya contenido
				while(readbytes != 0){
					//escribir en el archivo de destino todo
					write(archivo_destino, buffer, readbytes);

					//////////////////////////////////////////////////////////////////////
					//Esto es para escribir por pantalla lo leido y poder comprobar		//
					//que se pasa bien sin abrir el archivo                             //
					//////////////////////////////////////////////////////////////////////
					//buffer[readbytes] = '\0'; 										//
					//printf("%s", buffer);												//
					//////////////////////////////////////////////////////////////////////

					//leer siguiente linea		
					readbytes = read(tuberia[0], buffer, SIZE);				
				}
				//cerrar el archivo de destino
				close(archivo_destino);
				//cerrar la tuberia de lectura
				close(tuberia[0]);
			}
		}
	}	
	return 0;
}
