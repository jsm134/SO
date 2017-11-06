#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[]) {

	//if(argc == 3) {

		int socket_cliente, puerto, leido, escrito, tamano_buffer;

		struct sockaddr_in struct_servidor; // struct para almacenar la informacion sobre la dirección del servidor.
		struct hostent *host_remoto; // struct para almacenar la información sobre el host remoto.

		tamano_buffer = 500;
		char buffer[tamano_buffer];

		puerto = 9999;

		// Se crea un socket para el cliente.
		// AF_INET = para que las conexiones puedan ser o en distintos PC o en el mismo PC.
		// SOCK_STREAM = para que la comunicación sea orientada a conexión.
		// 0 = protocolo elegido.
		socket_cliente = socket(AF_INET, SOCK_STREAM, 0);

		if(socket_cliente == -1) {
			perror("Error al crear el socket del cliente.");
			exit(1);
		}

		// Para obtener el nombre del servidor.
		// 127.0.0.1 = dirección de loopback, para cuando el cliente y el servidor están en la misma máquina.
		host_remoto = gethostbyname("127.0.0.1");
		//host_remoto = gethostbyname(argv[1]); // Para coger el que se pasa como parámetro.

		if(host_remoto == NULL) {
			perror("Error, el host al que se quiere acceder no existe.");
			exit(1);
		}

		// Inicializa todos los campos del struct a 0.
		bzero((char *)&struct_servidor, sizeof(struct_servidor));

		// Se rellena el struct del servidor.
		// para que las conexiones puedan ser o en distintos PC o en el mismo PC.
		struct_servidor.sin_family = AF_INET;
		// el "int puerto" se convierte a formato máquina.
		struct_servidor.sin_port = htons(puerto);
		// Se copia la información del struct hostent en la información del servidor.
		// h_addr es la direccion del servidor.
		bcopy((char *)host_remoto->h_addr, (char *)&struct_servidor.sin_addr.s_addr, host_remoto->h_length);

		// Se establece conexión entre el cliente y el servidor.
		if(connect(socket_cliente, (struct sockaddr *)&struct_servidor, sizeof(struct_servidor)) == -1) {
			perror("Error al establecer la conexion con el servidor.");
			exit(1);
		}
/*
		printf("**** CLIENTE **** \n");
		printf("COMANDO PARA ENVIAR AL SERVIDOR: ");

		// Poner el buffer a 0.
		bzero(buffer, tamano_buffer);
		// Introducir por teclaro el comando.
		// Se introduce un char menos porque el ultimo char debe '\0'.
		fgets(buffer, tamano_buffer-1, stdin);
*//*
		bzero(buffer, tamano_buffer);
		strcpy(buffer, argv[2]);
		printf("Comando leído: %s \n", buffer);
*/
		// Escribir en el socket.
		char comando[100];
		printf("introduce el comando a mandar al servidor: ");
		scanf("%s", comando);
		escrito = write(socket_cliente, comando, strlen(comando) + 1);		
		// escrito = write(socket_cliente, argv[2], strlen(argv[2]) + 1);
		printf("Escrito: %d \n", escrito);
		if(escrito == -1) {
			perror("Error al escribir el comando en el socket.");
			exit(1);
		}
		printf("Aqui llego: buffer: %s \n", buffer);
		// Volver a poner el buffer a 0.
		bzero(buffer, tamano_buffer);

		printf("buffer: %s \n", buffer);
		// El cliente lee lo devuelto por el servidor.
		leido = read(socket_cliente, buffer, tamano_buffer-1);

		printf("\n");
		printf("Respuesta del servidor: \n");
		printf("\n");
		printf("%s \n", buffer);
	
		if(leido == -1) {
			perror("Error al leer la respuesta desde el socket.");
			exit(1);
		}
	
		close(socket_cliente);
//	}
	
//	else {
//		printf("Error, debe haber tres parámetros de entrada. \n");
//		printf("Ejecución: ./cliente [IP_servidor] [comando_a_ejecutar] \n");
//	}
}
