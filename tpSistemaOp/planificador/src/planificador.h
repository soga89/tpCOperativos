#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

// Includes
#include <delEquipo/sockets.h>
#include <delEquipo/archivoConfig.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Define

// Estructuras
typedef struct {
	int id;
	int cpu;
	int inicio;
	int cantidad;
	char ruta[200];
} pcb;

typedef struct {
	int id;
	int fd;
} cpu;

// Prototipos
void usoCpu(t_list* listaCpu, t_list* listaPcb);
// funcionesPlanificador
void setearValores(t_config * archivoConfig);
int testVGBienSeteadas(t_log * logger);
void leerComandos(t_list * listaProcesos, t_list * listaCPU);
void escucharACPU(t_list * listaCPU);
int perteneceId(t_list * unaLista, int unId);
int crearNumId(t_list * unaLista);

// Variables Globales
int vg_puerto_escucha;
int vg_quantum;

char * vg_algoritmo_planificacion;
#endif /* PLANIFICADOR_H_ */
