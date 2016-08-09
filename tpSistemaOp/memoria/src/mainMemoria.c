#include "memoria.h"

int main() {
// Creo el logger para la memoria
	char * logMemPath = strdup("/home/utnso/TPOperativos/logs/MEMORIA_LOG.log");
	char * logMemoNombreArch = strdup("MEMORIA_LOG.log");
	t_log * logger = malloc(sizeof(t_log));
	logger = log_create(logMemPath,logMemoNombreArch,false,LOG_LEVEL_INFO);
	free(logMemPath);
	free(logMemoNombreArch);

// Lee archivo de configuracion
	char * configPath = strdup("/home/utnso/TPOperativos/memoria/memoria.cfg");
	leerArchivoDeConfiguracion(configPath);
	free(configPath);
	if(comprobarValoresBienSeteados(logger)){
		log_info(logger,"Archivo de configuracion leido correctamente");
	}

// Conexiones
	int fdSwap = conectarASwap(logger);
	int fdCPU = escucharACPU(logger);

	int tipoMensaje, tamanioMensaje;
	void * entrada = recibirPaquete(fdCPU, &tipoMensaje, &tamanioMensaje);
	while (entrada != NULL){
		printf("Recibido: %s\n", (char *) entrada);
		enviarPaquete(fdSwap, 0, entrada, tamanioMensaje);
		free(entrada);
		entrada = recibirPaquete(fdCPU, &tipoMensaje, &tamanioMensaje);
	}

	cerrarSocket(fdSwap);
	cerrarSocket(fdCPU);
	return 0;
}
