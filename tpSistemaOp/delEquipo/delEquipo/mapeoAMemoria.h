#ifndef MAPEOAMEMORIA_H_
#define MAPEOAMEMORIA_H_

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>	// mmap

#include <stdlib.h>		// exit
#include <sys/stat.h>	// open  fstat
#include <sys/types.h>	// open  fstat
#include <fcntl.h>		// open
#include <unistd.h>		// fstat close
#include <errno.h>		// perror

#define FAIL -1

// Manejo de Archivos
int  abreArchivo(char* dirArchivo);
int  tamanio_archivo(int fd);
void cierraArchivo(int fdArchivo);

// Funciones mapeo
void  mapeoAmemoria(char* dirArchivo, char** ptrComienzoMemoriaMapeada, int* ptrTamanioDePagina);
	void * _mapeaAMemoria(int fdArchivo, int tamanioPagina);
void  desMapea(int tamanio, char* ptrAMapeo);

#endif /* MAPEOAMEMORIA_H_ */
