#include "mapeoAMemoria.h"

// ******************************
// * 	Manejo de Archivos 		*
// ******************************

int abreArchivo(char* dirArchivo) {
	int fd = open(dirArchivo, O_RDONLY);
	if (fd == FAIL) {
		perror("[ERROR] Error al abrir el archivo");
		exit(FAIL);
	}
	return fd;
}

int tamanio_archivo(int fd) {
	struct stat buf;
	fstat(fd, &buf);
	return buf.st_size;
}

void cierraArchivo(int fdArchivo) {
	if (close(fdArchivo) == FAIL) {
		perror("[ERROR] Funcion CLOSE: Error al cerrar el file descriptor\n");
		exit(FAIL);
	}
}



// **************************
// *	Funciones mapeo		*
// **************************

void mapeoAmemoria(char* dirArchivo, char** ptrComienzoMemoriaMapeada, int * ptrTamanioDePagina) {
	int fdArchivo = abreArchivo(dirArchivo);
	int tamanio = tamanio_archivo(fdArchivo);
	char * ptrAMapeo = (char*) _mapeaAMemoria(fdArchivo, tamanio);

	cierraArchivo(fdArchivo);
	ptrTamanioDePagina = &tamanio;
	*ptrComienzoMemoriaMapeada = ptrAMapeo;
	free(ptrAMapeo);
}

void * _mapeaAMemoria(int fdArchivo, int tamanioPagina) {
	void* ptrMapeo;
	ptrMapeo = mmap(NULL, tamanioPagina, PROT_READ, MAP_SHARED, fdArchivo, 0);
	if (ptrMapeo == MAP_FAILED) {
		perror("[ERROR] Funcion MMAP: Error al mapear el archivo\n");
		exit(FAIL);
	}
	return ptrMapeo;
}

void desMapea(int tamanio, char* ptrAMapeo) {
	if (munmap(ptrAMapeo, tamanio) == FAIL) {
		perror("[ERROR] Funcion MUNMAP: Error al desmapear memoria\n");
		exit(FAIL);
	}
}
