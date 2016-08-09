#ifndef ARCHIVOCONFIG_H_
#define ARCHIVOCONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>	// perror
#include <commons/config.h>

#define LONGPATH 200
#define FAIL -1

// Prototipos
void leerArchivoDeConfiguracion(char * configPath);
	int verificarExistenciaDeArchivo(char * ruta);
	void setearValores(t_config * archivoConfig); // Redefinir en cada proyecto
char * ingresarRuta(char * deQue);
void testLeerArchivoDeConfiguracion(); // Redefinir en cada proyecto. Son todos printf de los valores seteados



/*
 * Demostracion de uso de la funcion setearValores
 *
 * void setearValores(t_config * archivoConfig){
 * 		Copiar y Pegar el codigo correspondiente a cada tipo de dato
 * 		case int:
 * 			int variable = config_get_int_value(archivoConfig, "OBJETIVO");
 * 		case string:
 * 			char* variable = strdup(config_get_string_value(archivoConfig, "OBJETIVO"));
 * 		case long:
 * 			long variable = config_get_long_value(archivoConfig,"OBJETIVO");
 * 		case double
 * 			double variable = config_get_double_value(archivoConfig,"OBJETIVO");
 * 		case array:
 * 			char ** array = config_get_array_value(archivoConfig,"OBJETIVO");
 * }
 */

#endif /* ARCHIVOCONFIG_H_ */
