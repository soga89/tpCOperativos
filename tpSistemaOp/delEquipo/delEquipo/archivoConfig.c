#include "archivoConfig.h"
#define manejarError(msg) {perror(msg); abort();}

void leerArchivoDeConfiguracion(char * configPath) {
	t_config * archivoConfig;

	if (! verificarExistenciaDeArchivo(configPath))
		manejarError("[ERROR] Archivo de configuracion no encontrado");

	archivoConfig = config_create(configPath);
	setearValores(archivoConfig);
	free(configPath);
	config_destroy(archivoConfig);
}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	return (archivoConfig != NULL)? 1 : FAIL ;
}

char * ingresarRuta(char * deQue){
	char * path = malloc(sizeof(char) * 100);
	printf("Ingrese la ruta del archivo de %s: ",deQue);
	scanf("%s",path);
	free(deQue);
	return path;
}
