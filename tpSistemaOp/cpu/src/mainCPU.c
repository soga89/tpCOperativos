#include "cpu.h"

int main(){
// Log para el proceso CPU
	char * rutaDelLog = strdup("/home/utnso/TPOperativos/logs/CPU_LOG.log");
	char * logCPUFileName = strdup("CPU_LOG.log");
	t_log * logger = malloc(sizeof(t_log));
	logger = log_create(rutaDelLog,logCPUFileName,false,LOG_LEVEL_INFO);
	free(rutaDelLog);
	free(logCPUFileName);

// Lee archivo de configuracion
	char * configPath = strdup("/home/utnso/TPOperativos/cpu/cpu.cfg");
	leerArchivoDeConfiguracion(configPath);
	free(configPath);
	if( compruebaDatosBienSeteados(logger)){
		log_info(logger,"El proceso CPU leyo correctamente el archivo de configuración");
	}

// Conexiones
	int fdPlanificador = conectarAPlanificador(logger);
	log_info(logger,"El proceso CPU se conecto al planificador. fdPlanificador = %d",fdPlanificador);
	int fdMemoria = conectarAMemoria(logger);
	log_info(logger,"El proceso CPU se conecto a la memoria. fdMemoria = %d",fdMemoria);

	int tipoMensaje, tamanioMensaje;

// Recibe los int del header
	void * entrada = recibirPaquete(fdPlanificador, &tipoMensaje, &tamanioMensaje);
	while (entrada != NULL){
		switch (tipoMensaje)
		{
			case CREAR_PCB:
			{
				pcb * nuevoPcb = (pcb *) entrada;
				printf("Ejecutar PID: %i - CPU: %i (%s)\n", nuevoPcb->id, nuevoPcb->cpu ,nuevoPcb->ruta);
				enviarPaquete(fdMemoria, 0, &nuevoPcb->ruta, sizeof(nuevoPcb->ruta));
				break;
			}

			default:
			{
				log_error(logger,"CPU recibio como tipo de mensaje %d y no reconoce ese tipo",tipoMensaje);
				perror("[Error] Tipo de mensaje no reconocido");
			}
		}
		free(entrada);
		entrada = recibirPaquete(fdPlanificador, &tipoMensaje, &tamanioMensaje);
	}

	cerrarSocket(fdPlanificador);
	cerrarSocket(fdMemoria);

	log_destroy(logger);

	return 0;
}
