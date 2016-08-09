#include "redireccionIO.h"

int ejecutarScriptRedireccionandoIO(char * dirArchivoSalida, char * ptrDireccionMapeo,
								    char * dirScript, char * nombreScript) {
	pid_t pid;
	if( pipe(pipes[ESCRITURA_PADRE]) == FAIL){
		manejaError("[ERROR] En la creacion del pipe");
	}

	pid = fork();

	if (pid < 0){
		manejaError("[ERROR] En la funcion fork");
	}

	if (pid == 0){
		ejecutaProcesoHijo(dirArchivoSalida, dirScript, nombreScript);
	}

	if (pid > 0){
		ejecutarProcesoPadre(ptrDireccionMapeo);
	}

	return 0;
}

void ejecutaProcesoHijo(char * dirArchivoSalida, char * dirScript, char * nombreScript) {

	char *parametrosScript[] = { nombreScript, NULL };

	// 1) Abro el archivo que voy a usar como salida
	int fdArchivoSalida = open(dirArchivoSalida, O_RDWR | O_CREAT);
	if (fdArchivoSalida == FAIL) {
		manejaError("[ERROR] Al abrir el archivo de salida");
	}

	// 2) Duplico el fd de lectura del hijo al fd de la entrada standar
	if ( dup2(CANAL_LECTURA_HIJO, STDIN_FILENO) == FAIL) {
		manejaError("[ERROR] No se pudo duplicar el File Descriptor para la entrada");
	}

	// 3) Duplico el fd de escritura del hijo al fd de la salida standar
	if (dup2(fdArchivoSalida, STDOUT_FILENO) == FAIL) {
		manejaError("[ERROR] No se pudo duplicar el File Descriptor para la salida");
	}

	// 4) Cierro los pipes que no se van a usar
	if (close(CANAL_ESCRITURA_PADRE) == FAIL) {
		manejaError("[ERROR] No se cerro correctamente el pipe de escritura");
	}
	if (close(CANAL_LECTURA_HIJO) == FAIL) {
		manejaError("[ERROR] No se cerro correctamente el pipe de escritura");
	}

	// 5) Ejecuto proceso en argv[0]
	if (execv(dirScript, parametrosScript) == FAIL) {
		manejaError("[ERROR] No se pudo ejecutar el script ingresado");
	}
}

void ejecutarProcesoPadre(char * ptrDireccionMapeo){
  // 1) Cierro los pipes que le hijo no va a usar
	close(CANAL_LECTURA_HIJO);

  // 2) Recibo direccion de memoria del mapeo y lo mando por pipe al hijo
	if (write(CANAL_ESCRITURA_PADRE, ptrDireccionMapeo,	sizeof(ptrDireccionMapeo)) == FAIL){
		manejaError("[ERROR] El padre no pudo escribir en el pipe");
	}
}

/* Esto es un ejemplo de como se puede llamar a la funcion
 int main() {
 	 printf("Ejecutando \n");

 	 if (ejecutarScriptRedireccionandoIO("Salida.txt", "Entrada.txt", "/usr/bin/sort", "") == 0) {
 	 	 printf("Ejecucion correcta \n");
 	 }
 	 return 0;
 }
 */
