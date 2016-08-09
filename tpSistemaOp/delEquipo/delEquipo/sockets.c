#include "sockets.h"

// *******************************
// *           Servidor			 *
// *******************************
int newSocket() {
	int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		if (fileDescriptor == FAIL) manejaError("[ERROR] Funcion socket");
	return fileDescriptor;
}

struct sockaddr_in asociarSocket(int sockfd, int puerto) {
	struct sockaddr_in miDireccion;
	miDireccion.sin_family = AF_INET;
	miDireccion.sin_port = htons(puerto);
	miDireccion.sin_addr.s_addr = 0; // htonl(INADDR_ANY); // Usa mi direccion IP
	memset(&(miDireccion.sin_zero), '\0', 8);

	// Si el puerto esta en uso, lanzamos error
	int enUso = 1;
	int estaEnUso = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &enUso, sizeof(enUso));
		if (estaEnUso == FAIL)	manejaError("[ERROR] No es posible reutilizar el socket");
	
	// Funcion bind
	int funcionBind = bind(sockfd, (struct sockaddr *) &miDireccion,sizeof(struct sockaddr));
		if ( funcionBind == FAIL) manejaError("[ERROR] Funcion BIND: No se pudo asociar con el puerto");

	return miDireccion;
}

void escucharSocket(int sockfd, int conexionesEntrantesPermitidas) {
	int funcionListen = listen(sockfd, conexionesEntrantesPermitidas);
		if ( funcionListen == FAIL) manejaError("[ERROR] Funcion listen");
}

int aceptarConexionSocket(int sockfd) {
	struct sockaddr_storage cliente;
	unsigned int addres_size = sizeof(cliente);

	int fdCliente = accept(sockfd, (struct sockaddr*) &cliente, &addres_size);
		if(fdCliente == FAIL) manejaError("[ERROR] Funcion accept");

	return fdCliente;
}

// *******************************
// *           Cliente			 *
// *******************************
struct sockaddr_in conectarSocket(int sockfd, const char * ipDestino, int puerto){
	struct sockaddr_in datosServidor;

	datosServidor.sin_family = AF_INET;
	datosServidor.sin_port = htons(puerto);
	datosServidor.sin_addr.s_addr = inet_addr(ipDestino);
	memset(&(datosServidor.sin_zero), '\0', 8);

	int funcionConnect = connect(sockfd, (struct sockaddr *) &datosServidor, sizeof(struct sockaddr));
		if ( funcionConnect == FAIL) manejaError("[ERROR] Funcion connect");
	return datosServidor;
}

// ***************************************
// *       Enviar y Recibir	Paquetes	 *
// ***************************************
/* Comentado porque no compila
 *
 * void enviarPaquete (int fdCliente,int tipoDeMensaje,void* payload,int tamanioPayload){
	PaqueteEnvio* unPaquete = prepararPaqueteAEnviar(tipoDeMensaje,payload,tamanioPayload);
	enviarPorSocket(fdCliente,&(unPaquete->tamanioMensaje),sizeof(unPaquete->tamanioMensaje));
	enviarPorSocket(fdCliente,(unPaquete->mensaje),unPaquete->tamanioMensaje);
}

PaqueteRecv* recibirPaquete(int fdCliente){
	size_t tamanioMensaje;
	recibirPorSocket(fdCliente,&tamanioMensaje,sizeof(int));
	void* mensaje = malloc(tamanioMensaje);
	recibirPorSocket(fdCliente,mensaje,tamanioMensaje);
	PaqueteRecv* unPaquete = abrirPaqueteRecv(mensaje,tamanioMensaje);
	return unPaquete;
}
*/

//agrego prototipo de Enviar y recibir paquetes serializando los datos , haciendo un solo envio y asegurando la transaccionalidad
//las dos funciones son prototipos por lo lo que se requiere hacer pruebas de test

void enviarPaqueteSerializado(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje){

	void * msjSerializado=malloc(tamanioMensaje+2*sizeof(int));
	memcpy(msjSerializado,&tipoMensaje,4);
	memcpy(msjSerializado+4,&tamanioMensaje,4);
	memcpy(msjSerializado+8,mensaje,tamanioMensaje);
	enviarPorSocket(fdCliente, msjSerializado,tamanioMensaje+2*sizeof(int));

}


void * recibirPaqueteSerializado(int fdCliente, int * tipoMensaje, int * tamanioMensaje){

	* tipoMensaje=malloc(sizeof(int));
	* tamanioMensaje=malloc(sizeof(int));
	int recibido = recibirPorSocket(fdCliente,(int*) tipoMensaje, sizeof(int));
	if (recibido > 0) {

		recibido = recibirPorSocket(fdCliente, (int*)tamanioMensaje, sizeof(int));
		if (recibido > 0) {

				void * buffer = malloc(*tamanioMensaje);
				recibido = recibirPorSocket(fdCliente, buffer, *tamanioMensaje);
					if (recibido > 0) {

							return buffer;
							}
						}

	}
	return NULL;
}



void enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje){
	cabeceraMensaje nuevoMensaje;
	nuevoMensaje.tipo = tipoMensaje;
	nuevoMensaje.tamanio = tamanioMensaje;
	enviarPorSocket(fdCliente, (void *) &nuevoMensaje, sizeof(cabeceraMensaje));
	enviarPorSocket(fdCliente, mensaje, nuevoMensaje.tamanio);
}

void * recibirPaquete(int fdCliente, int * tipoMensaje, int * tamanioMensaje){
	cabeceraMensaje nuevoMensaje;
	int recibido = recibirPorSocket(fdCliente, &nuevoMensaje, sizeof(cabeceraMensaje));
	if (recibido > 0) {
		void * buffer = malloc(nuevoMensaje.tamanio);
		recibido = recibirPorSocket(fdCliente, buffer, nuevoMensaje.tamanio);
		if (recibido > 0) {
			*tipoMensaje = nuevoMensaje.tipo;
			*tamanioMensaje = nuevoMensaje.tamanio;
			return buffer;
		}
	}
	return NULL;
}


// *******************************
// *       Enviar y Recibir		 *
// *******************************

int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio) {
	int bytes_enviados;
	int total = 0;

	while (total < tamanio) {
		bytes_enviados = send(fdCliente, mensaje + total, tamanio, 0);
		if (bytes_enviados == FAIL) {
			break;
		}
		total += bytes_enviados;
		tamanio -= bytes_enviados;
	}

	if (bytes_enviados == FAIL) manejaError ("[ERROR] Funcion send");
	
	return bytes_enviados;
}

int recibirPorSocket(int fdCliente, void * buffer, int tamanio) {
	int total = 0;
	int bytesRecibidos;

	while (total < tamanio) {
		bytesRecibidos = recv(fdCliente, buffer + total, tamanio, 0);
		if (bytesRecibidos == FAIL) {
			// Error
			perror("[ERROR] Funcion recv");
			break;
		}
		if (bytesRecibidos == 0) {
			// Desconexion
			break;
		}
		total += bytesRecibidos;
		tamanio -= bytesRecibidos;
	}
	return bytesRecibidos;
}	// retorna -1 si fallo, 0 si se desconecto o los bytes recibidos

// *******************************
// *    Cerrar File Descriptor	 *
// *******************************
void cerrarSocket(int sockfd) {
	int funcionClose = close(sockfd);
		if (funcionClose == FAIL) manejaError("[ERROR] Funcion CLOSE: Error al cerrar el file descriptor");
}

// *******************************
// *       	  Select			 *
// *******************************
void seleccionarSocket(int maxNumDeFD,
					   fd_set *fdListoLectura, fd_set *fdListoEscritura, fd_set *fdListoEjecucion,
					   int* segundos, int* milisegundos) {

	if(segundos == NULL || milisegundos == NULL){
		// Si segundos = NULL o miliSegundos = NULL => espera infinitamente
		int funcionSelect = select((maxNumDeFD + 1), fdListoLectura, fdListoEscritura, fdListoEjecucion, NULL);
		if(funcionSelect == FAIL) manejaError("[ERROR] Funcion select");
	}
	else{
		struct timeval tv;
		// Si segundos y milisegundos apuntan a 0 => select regresa inmediatamente despues de interrogar a todos sus fd
		int sec = *segundos;
		int mic = (*milisegundos) * 1000;
		tv.tv_sec  = sec;
		tv.tv_usec = mic;

		int selectConTiempo = select((maxNumDeFD + 1), fdListoLectura,
								  fdListoEscritura, fdListoEjecucion, &tv);

		if (selectConTiempo == FAIL) manejaError("[ERROR] Funcion select");
	}
}
