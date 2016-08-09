#ifndef SOCKETS_H_
#define SOCKETS_H_

#include "protocolo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include <fcntl.h> //fcntl

#define FAIL -1
#define manejaError(msj) {perror(msj); abort();}

// Servidor
int  newSocket();
struct sockaddr_in asociarSocket(int sockfd, int puerto);
void escucharSocket(int sockfd, int conexionesEntrantesPermitidas);
int  aceptarConexionSocket(int sockfd);

// Cliente
struct sockaddr_in conectarSocket(int sockfd, const char * ipDestino, int puerto);

int enviarPorSocketSerializado(int fdCliente, const void * mensaje, int tamanio);
int recibirPorSocketSerializado(int fdCliente, void * buffer, int tamanio) ; // retorna -1 si fallo, 0 si se desconecto o los bytes recibidos


// Enviar y Recibir
int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio);
int recibirPorSocket(int fdCliente, void * buffer, int tamanio) ; // retorna -1 si fallo, 0 si se desconecto o los bytes recibidos

//EnviarPaquete y RecibirPaquete
/* Comentado porque no compila
void enviarPaquete (int fdCliente,int tipoDeMensaje,void* payload,int tamanioPayload);
PaqueteRecv* recibirPaquete(int fdCliente);
*/
typedef struct{
	int tipo;
	int tamanio;
} cabeceraMensaje;
void enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje);
void * recibirPaquete(int fdCliente, int * tipoMensaje, int * tamanioMensaje);

void enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje);
void* recibirPaquete(int fdCliente, int * tipoMensaje, int * tamanioMensaje);

// Cerrar File Descriptor
void cerrarSocket(int sockfd);

// Select
void seleccionarSocket(int maxNumDeFD,
					   fd_set * fdListoLectura, fd_set * fdListoEscritura, fd_set * fdListoEjecucion,
					   int* segundos, int* miliSegundos);

#endif /* SOCKETS_H_ */
