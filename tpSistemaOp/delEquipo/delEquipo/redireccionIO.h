#ifndef REDIRECCIONIO_H_
#define REDIRECCIONIO_H_

// ******************
// *    Includes	*
// ******************
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// ******************
// *     Define	    *
// ******************
#define NUMERO_PIPES    1
#define NUMERO_CANALES  2

#define LECTURA_PADRE   0
#define ESCRITURA_PADRE 1

#define ESCRIBIR 1
#define LEER     0

#define CANAL_ESCRITURA_PADRE (pipes[ESCRITURA_PADRE][ESCRIBIR])
#define CANAL_LECTURA_HIJO    (pipes[ESCRITURA_PADRE][LEER])

#define FAIL -1
#define manejaError(msj) {perror(msj); exit(FAIL);}

// ******************
// *   Prototipos	*
// ******************
int ejecutarScriptRedireccionandoIO(char * dirArchivoSalida, char * ptrDireccionMapeo,
								    char * dirScript, char * nombreScript);
	void ejecutaProcesoHijo(char * dirArchivoSalida, char * dirScript, char * nombreScript);
	void ejecutarProcesoPadre(char * ptrDireccionMapeo);

// **************************
// *   Variables Globales	*
// **************************
int pipes[NUMERO_PIPES][NUMERO_CANALES];

#endif /* REDIRECCIONIO_H_ */
