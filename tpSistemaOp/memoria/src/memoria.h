#ifndef MEMORIA_H_
#define MEMORIA_H_

// Includes
#include <delEquipo/sockets.h>
#include <delEquipo/archivoConfig.h>
#include <string.h>
#include <commons/log.h>
#include <ctype.h>


// Define

// Estructuras
typedef struct {
	char presente;
	char modificada;
	int posicion_en_memoria;
} elemento_lista_de_paginas;

typedef struct {
	int PID;
	int pagina;
	char * valor;
} elemento_memoria;

typedef struct {
	int PID;
	int pagina;
	int posicion_en_memoria;
} elemento_TLB;

// Prototipos
// funcionesMemoria.c
void setearValores(t_config * archivoConfig);
int conectarASwap(t_log * logger);
int escucharACPU(t_log * logger);
int comprobarValoresBienSeteados(t_log * logger);

// Variables Globales
int vg_puerto_escucha;
char * vg_ip_swap;
int vg_puerto_swap;
int vg_maximo_marcos_por_proceso;
int vg_cantidad_marcos;
int vg_tamanio_marco;
int vg_entradas_tlb;
char * vg_tlb_habilitada;
int vg_retardo_memoria;

#endif /* MEMORIA_H_ */
