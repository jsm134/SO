#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {

	if(argc == 1) {

		int socket_servidor, socket_cliente, puerto, leido, max_clientes, tamano_buffer;

		socklen_t long_cliente;

		tamano_buffer = 500;
		char buffer[tamano_buffer]; // buffer para almacenar los char leídos de los comandos.

		struct sockaddr_in struct_servidor; // struct para almacenar la información del servidor.
		struct sockaddr_in struct_cliente; // struct para almacenar la información del cliente.
	
		puerto = 9999;
		max_clientes = 10;
		long_cliente = sizeof(struct_cliente);
	
		// Se crea un socket para el servidor.
		// AF_INET = para que las conexiones puedan ser o en distintos PC o en el mismo PC.
		// SOCK_STREAM = para que la comunicación sea orientada a conexión.
		// 0 = protocolo elegido.
		socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
	
		if(socket_servidor == -1) {
			perror("Error al crear el socket del servidor.");
			exit(1);
		}

		// Inicializa todos los campos del struct a 0.
		bzero((char *)&struct_servidor, sizeof(struct_servidor));

		// Se rellena el struct del servidor.
		// para que las conexiones puedan ser o en distintos PC o en el mismo PC.
		struct_servidor.sin_family = AF_INET;
		// el "int puerto" se convierte a formato máquina.
		struct_servidor.sin_port = htons(puerto);
		// para que se pueda aceptar cualquier dirección.
		struct_servidor.sin_addr.s_addr = INADDR_ANY;

		// Se asocia el puerto con el socket del servidor.
		if(bind(socket_servidor, (struct sockaddr*)&struct_servidor, sizeof(struct_servidor)) == -1) {
			perror("Error en la llamada bind().");
			exit(1);
		}

		// Se mira si hay alguna conexión entrante en el puerto.
		listen(socket_servidor, max_clientes);

		// Se acepta al primer cliente que haya pedido conexión y esté en la cola.
		do{
			socket_cliente = accept(socket_servidor, (struct sockaddr*)&struct_cliente, &long_cliente); // espero un cliente
			if(socket_cliente == -1) {
				perror("Error al aceptar la conexion del cliente.");
				exit(1);
			}

			// Poner el buffer a 0.
			bzero(buffer, tamano_buffer);
			// se lee un char menos porque el ultimo char debe ser '\0'.
			leido = read(socket_cliente, buffer, tamano_buffer-1);
			printf("Leido: %d \n", leido);
			printf("Buffer: %s \n", buffer);

			if (leido == -1) {
				perror("Error al leer el comando desde el socket.");
				exit(1);
			}
//	tabla de descriptores de ficheros
//	0	std_in 		std_in		
//	1	std out		free => socket_cliente
//	2	std_out		std out


			// Se crea un hijo para que se pueda ejecutar el comando leido.
			int pid = fork();

			if(pid == 0){ // Si el proceso es el hijo.
				// Se cierra la salida por pantalla (1 = salida por pantalla).
				close(1);
				// Se añade el primer descriptor disponible al socket para que el programa escriba en el socket.
				/* Con close() se libera un descriptor, por eso se sabe que dup() devolverá el descriptor
				 * de fichero que se acaba de liberar, así que se puede cerrar el descriptor porque a la
				 * salida por pantalla ya apunta el nuevo descriptor.
				 */
				dup(socket_cliente);
				// Se  ejecuta el comando leído.
				execlp((const char*)buffer, (const char*)buffer, NULL);
			}

			else{
				buffer[strlen(buffer)-1] = '\0';
				if(leido == -1) {
					perror("Error al ejecutar el comando desde el socket.");
					exit(1);
				}
			}

		// Si un cliente envía el comando "exit", el servidor cierra la conexión.
		} while(strcmp(buffer, "exit") != 0);

		// Sólo llegará aquí cuando acabe el bucle
		printf("\n");
		printf("Soy el servidor y cierro la conexión con el cliente. \n");
		printf("\n");

		close(socket_cliente);
		close(socket_servidor);
	}

	else {
		printf("Error, sólo debe haber un parámetro de entrada. \n");
		printf("Ejecución: ./servidor \n");
	}
}
