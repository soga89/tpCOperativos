#include "protocolo.h"
//objetivo devolver el paquete para el envio
/*
PaqueteEnvio* prepararPaqueteAEnviar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad){
	PaqueteEnvio* paqueteAEnviar = malloc(sizeof(PaqueteEnvio));
	size_t tamanioDelMensaje = tamanioPayLoad + sizeof(int); //+4 por el total del mesnaje, le sumo un sizeof(int) por el header

	paqueteAEnviar->mensaje = malloc(tamanioDelMensaje);
	memcpy(paqueteAEnviar->mensaje,&tipoDeMensaje,sizeof(int));
	size_t desplazameinto = sizeof(int);
	memcpy(paqueteAEnviar->mensaje + desplazameinto,payLoad,tamanioPayLoad);

	paqueteAEnviar->tamanioMensaje = tamanioDelMensaje;
	return paqueteAEnviar;
}
// devolver el paquete para recepcion( y luego interpretar el paquete)
PaqueteRecv* abrirPaqueteRecv(void* mensajeRecv, int tamnioMensaje) {
	PaqueteRecv* paqueteDeserial = malloc(sizeof(PaqueteRecv));
	memcpy(&(paqueteDeserial->tipoDeMensaje),mensajeRecv,sizeof(int));
	int dezplazamiento = sizeof(int);
	size_t tamanioALeerDelBuffer = tamnioMensaje - dezplazamiento;
	paqueteDeserial->payLoad = malloc(tamanioALeerDelBuffer);
	memcpy(paqueteDeserial->payLoad, mensajeRecv + dezplazamiento,tamanioALeerDelBuffer);
	return paqueteDeserial;
}

*/
