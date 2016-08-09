#include "planificador.h"

int main() {
// Creo el logger para el planificador
	char * logPlanifPath = strdup("/home/utnso/TPOperativos/logs/PLANIFICADOR_LOG.log");
	char * logPlanifNombreArch = strdup("PLANIFICADOR_LOG.log");
	t_log * logger = malloc(sizeof(t_log));
	logger = log_create(logPlanifPath,logPlanifNombreArch,false,LOG_LEVEL_INFO);
	free(logPlanifPath);
	free(logPlanifNombreArch);

// Lee archivo de configuracion
	char * configPath = strdup("/home/utnso/TPOperativos/planificador/planificador.cfg");
	leerArchivoDeConfiguracion(configPath);
	if(testVGBienSeteadas(logger)){
		log_info(logger,"Archivo de configuracion leido correctamente");
	}
	free(configPath);

	t_list * listaProcesos = list_create();

	pthread_t  p_thread;
	t_list * listaCPU = list_create();

	int thr_id = pthread_create(&p_thread, NULL, escucharACPU, listaCPU);

	leerComandos(listaProcesos, listaCPU);

	return 0;
}
