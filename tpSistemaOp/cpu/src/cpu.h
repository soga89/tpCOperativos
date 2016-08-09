#ifndef CPU_H_
#define CPU_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <delEquipo/archivoConfig.h>
#include <delEquipo/sockets.h>
#include <commons/log.h>
#include <ctype.h> // isDigit

// Define
typedef enum{CREAR_PCB = 1} t_protocolo;

// Estructuras
typedef struct {
	int id;
	int cpu;
	int inicio;
	int cantidad;
	char ruta[200];
} pcb;

// Prototipos
// funciones CPU
void setearValores(t_config * archivoConfig);
int conectarAPlanificador(t_log * logger);
int conectarAMemoria(t_log * logger);

int compruebaDatosBienSeteados(t_log * logger);


// Variables Globales

char * vg_ip_memoria;
int vg_puerto_memoria;

char * vg_ip_planificador;
int vg_puerto_planificador;

int vg_cantidad_hilos;

int vg_retardo;

#endif /* CPU_H_ */
