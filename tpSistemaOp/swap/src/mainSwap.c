#include "swap.h"

int main() {
// Creo el logger para swap
	char * logSwapPath = strdup("/home/utnso/TPOperativos/logs/SWAP_LOG.log");
	char * logSwapNomArch = strdup("SWAP_LOG.log");
	t_log * logger = malloc(sizeof(t_log));
	logger = log_create(logSwapPath,logSwapNomArch,false,LOG_LEVEL_INFO);
	free(logSwapPath);
	free(logSwapNomArch);

// Lee archivo de configuracion
	char * configPath = strdup("/home/utnso/TPOperativos/swap/swap.cfg");
	leerArchivoDeConfiguracion(configPath);
	free(configPath);
	if(testVGBienSeteadas(logger)){
		log_info(logger,"Archivo de configuracion de swap leido correctamente");
	}

	FILE * disco = fopen(vg_nombre_swap, "wb+");

	formatear(disco);

	t_list * listaEspacioLibre = list_create();
	t_list * listaEspacioAsignado = list_create();

	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));

	nuevoEspacioLibre->inicio = 0;
	nuevoEspacioLibre->tamanio = vg_cantidad_paginas;

	list_add(listaEspacioLibre, nuevoEspacioLibre);

	int fdMemoria = escucharAMemoria();

	if (fdMemoria != 0){
		int tipoMensaje, tamanioMensaje;
		void * entrada = recibirPaquete(fdMemoria, &tipoMensaje, &tamanioMensaje);
		while (entrada != NULL){
			printf("Recibido: %s\n", (char *) entrada);
			free(entrada);
			entrada = recibirPaquete(fdMemoria, &tipoMensaje, &tamanioMensaje);
		}
	}

	cerrarSocket(fdMemoria);
	fclose(disco);
	return 0;
}
