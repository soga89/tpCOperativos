#ifndef SWAP_H_
#define SWAP_H_

// Includes
#include <string.h>
#include <delEquipo/sockets.h>
#include <delEquipo/archivoConfig.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <ctype.h>

// Define

// Estructuras
typedef struct{
	int inicio;
	int tamanio;
}espacioLibre;

typedef struct{
	int pid;
	int inicio;
	int tamanio;
}espacioAsignado;

// Prototipos

// funciones.c
void setearValores(t_config * archivoConfig);
int testVGBienSeteadas(t_log * logger);
void formatear(FILE * disco);
int hayEspacio(t_list * listaEspacioLibre, int tamanio);
int crearEspacio(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int tamanio);
void eliminarEspacio(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid);
void desfragmentar(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco);
void escribir(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int posicion, char * cadena);
char * leer(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int posicion);
int escucharAMemoria();

// Variables Globales
int vg_puerto_escucha;
char * vg_nombre_swap;
int vg_cantidad_paginas;
int vg_tamanio_pagina;
int vg_retardo_compactacion;

#endif /* SWAP_H_ */
