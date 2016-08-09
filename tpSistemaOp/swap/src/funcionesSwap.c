#include "swap.h"

void setearValores(t_config * archivoConfig){
	vg_puerto_escucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
	vg_nombre_swap = strdup(config_get_string_value(archivoConfig, "NOMBRE_SWAP"));
	vg_cantidad_paginas = config_get_int_value(archivoConfig, "CANTIDAD_PAGINAS");
	vg_tamanio_pagina = config_get_int_value(archivoConfig, "TAMANIO_PAGINA");
	vg_retardo_compactacion = config_get_int_value(archivoConfig, "RETARDO_COMPACTACION");
}
int testVGBienSeteadas(t_log * logger){
	int retorno = 1;
	if(isdigit(vg_puerto_escucha) == 0){
		log_error(logger,"testVGBienSeteados() - vg_puerto_escucha no es un int o esta mal seteado");
		retorno = 0;
	}
	if(isdigit(vg_cantidad_paginas) == 0){
		log_error(logger,"testVGBienSeteados() - vg_cantidad_paginas no es un int o esta mal seteado");
		retorno = 0;
	}
	if(isdigit(vg_retardo_compactacion) == 0){
		log_error(logger,"testVGBienSeteados() - vg_retardo_compactacion no es un int o esta mal seteado");
		retorno = 0;
	}
	if(isdigit(vg_tamanio_pagina) == 0){
		log_error(logger,"testVGBienSeteados() - vg_tamanio_pagina no es un int o esta mal seteado");
		retorno = 0;
	}
	if(vg_nombre_swap == NULL){
		log_error(logger,"testVGBienSeteados() - vg_nombre_swap vale NULL");
		retorno = 0;
	}
	return retorno;
}
/*
 * Formatea el disco asignadole '\0' a cada posicion
 *
 */
void formatear(FILE * disco){
	int i, j;
	char dato = '\0';
	for (i = 0; i < vg_cantidad_paginas; i++){
		for (j = 0; j < vg_tamanio_pagina; j++){
			fwrite(&dato, 1, 1, disco);
		}
	}
}

/*
 * Verifica si hay espacio disponible. Si hay devuelve la el indice del struct de la posicion libre, sino devuelve -1
 *
 */
int hayEspacio(t_list * listaEspacioLibre, int tamanio){
	int i;
	for(i = 0; i < list_size(listaEspacioLibre); i++){
		espacioLibre * espacio = (espacioLibre *) list_get(listaEspacioLibre, i);
		if (espacio->tamanio >= tamanio){
			return i;
		}
	}
	return -1;
}

/*
 * Crea espacio para un proceso, retorna -1 si hubo error, 1 si fue exitoso
 */
int crearEspacio(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int tamanio){
	int lugarLibre = hayEspacio(listaEspacioLibre, tamanio);
	if (lugarLibre == -1){
		desfragmentar(listaEspacioLibre, listaEspacioAsignado, disco);
		lugarLibre = hayEspacio(listaEspacioLibre, tamanio);
	}


	if (lugarLibre != -1){
		espacioLibre * espacioLibreAnt = (espacioLibre *)list_get(listaEspacioLibre ,lugarLibre);

		espacioAsignado * nuevoEspacioAsignado = malloc(sizeof(espacioAsignado));

		nuevoEspacioAsignado->pid = pid;
		nuevoEspacioAsignado->inicio = espacioLibreAnt->inicio;
		nuevoEspacioAsignado->tamanio = tamanio;

		list_add(listaEspacioAsignado, nuevoEspacioAsignado);

		espacioLibreAnt->inicio = espacioLibreAnt->inicio + tamanio;
		espacioLibreAnt->tamanio = espacioLibreAnt->tamanio - tamanio;

		return 1;

	}
	return -1;
}

/*
 * Elimina el espacio de un proceso
 */
void eliminarEspacio(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid){
	int i;
	for (i = 0; i < list_size(listaEspacioAsignado); i++){
		espacioAsignado * espacio = (espacioAsignado *)list_get(listaEspacioAsignado, i);
		if (espacio->pid==pid){
			int j, asignado = 0;
			for (j = 0; j < list_size(listaEspacioLibre); j++){
				espacioLibre * unEspacioLibre = (espacioLibre *)list_get(listaEspacioLibre, j);

				if (unEspacioLibre->inicio + unEspacioLibre->tamanio == espacio->inicio){
					asignado = 2;
				}else if (espacio->inicio + espacio->tamanio == unEspacioLibre->inicio){
					unEspacioLibre->inicio = unEspacioLibre->inicio - espacio->tamanio;
					asignado = 2;
				}
				if (asignado == 2){
					unEspacioLibre->tamanio = unEspacioLibre->tamanio + espacio->tamanio;
					int k;
					for (k = 0; k < list_size(listaEspacioLibre); k++){
						espacioLibre * otroEspacioLibre = (espacioLibre *)list_get(listaEspacioLibre, k);

						if (otroEspacioLibre->inicio == unEspacioLibre->inicio + unEspacioLibre->tamanio){
							unEspacioLibre->tamanio = unEspacioLibre->tamanio + otroEspacioLibre->tamanio;
							list_remove(listaEspacioLibre, k);
						}else if (otroEspacioLibre->inicio + otroEspacioLibre->tamanio == unEspacioLibre->inicio){
							unEspacioLibre->inicio = otroEspacioLibre->inicio;
							unEspacioLibre->tamanio = unEspacioLibre->tamanio + otroEspacioLibre->tamanio;
							list_remove(listaEspacioLibre, k);
						}

					}
					asignado = 1;
				}

			}

			if (asignado==0){
				espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));
				nuevoEspacioLibre->inicio = espacio->inicio;
				nuevoEspacioLibre->tamanio = espacio->tamanio;

				list_add(listaEspacioLibre, nuevoEspacioLibre);
			}
			char dato = '\0';
			long int offset = espacio->inicio * vg_tamanio_pagina;
			fseek(disco, offset, SEEK_SET);
			int k;
			for (j = 0; j < espacio->tamanio ; j++){
				for (k = 0; k < vg_tamanio_pagina; k++){
					fwrite(&dato, 1, 1, disco);
				}
			}
			list_remove(listaEspacioAsignado, i);

		}
	}
}

/*
 * Desfragmenta el disco
 *
 */
void desfragmentar(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco){
	int i, j, comienzoMenor = 0, finMenor = vg_cantidad_paginas, indiceMenor = -1;
	for (i = 0; i < list_size(listaEspacioAsignado); i++){
		for (j = 0; j < list_size(listaEspacioAsignado); j++){
			espacioAsignado * unEspacio = (espacioAsignado * )list_get(listaEspacioAsignado, j);

			if (unEspacio->inicio >= comienzoMenor && unEspacio->inicio < finMenor){
				indiceMenor = j;
				finMenor = unEspacio->inicio;
			}
		}
		espacioAsignado * unEspacio = (espacioAsignado * )list_get(listaEspacioAsignado, indiceMenor);
		int comienzoAnterior = unEspacio->inicio;
		for (j = 0; j < unEspacio->tamanio; j++){
			unEspacio->inicio = comienzoAnterior;
			char * lectura = leer(listaEspacioLibre,listaEspacioAsignado, disco, unEspacio->pid, j);
			unEspacio->inicio = comienzoMenor;
			escribir(listaEspacioLibre, listaEspacioAsignado, disco, unEspacio->pid, j, lectura);
		}
		comienzoMenor += unEspacio->tamanio;
		finMenor = vg_cantidad_paginas;

	}
	list_clean(listaEspacioLibre);
	espacioLibre * nuevoEspacioLibre = malloc(sizeof(espacioLibre));

	nuevoEspacioLibre->inicio = comienzoMenor;
	nuevoEspacioLibre->tamanio = vg_cantidad_paginas - comienzoMenor;

	list_add(listaEspacioLibre, nuevoEspacioLibre);

	char dato = '\0';
	fseek(disco, nuevoEspacioLibre->inicio * vg_tamanio_pagina, SEEK_SET);
	for (i = 0; i < nuevoEspacioLibre->tamanio ; i++){
		for (j = 0; j < vg_tamanio_pagina; j++){
			fwrite(&dato, 1, 1, disco);
		}
	}
}

/*
 * Escribe en la posicion dada del proceso dado
 *
 */
void escribir(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int posicion, char * cadena){
	int i;
	for (i = 0; i < list_size(listaEspacioAsignado); i++){
		espacioAsignado * espacio = (espacioAsignado *)list_get(listaEspacioAsignado, i);
		if (espacio->pid==pid){

			int j = 0;
			fseek(disco, (espacio->inicio + posicion) * vg_tamanio_pagina, SEEK_SET);
			while (*cadena!='\0'){
				fwrite(cadena, 1, 1, disco);
				cadena++;
				j++;
			}
			int k;
			for (k = j; k < vg_tamanio_pagina; k++){
				fwrite(cadena, 1, 1, disco);
			}
		}
	}

}

/*
 * Lee de la posicion dada del proceso dado
 *
 */
char * leer(t_list * listaEspacioLibre, t_list * listaEspacioAsignado, FILE * disco, int pid, int posicion){
	int i;
	char * lectura = malloc(vg_tamanio_pagina);
	for (i = 0; i < list_size(listaEspacioAsignado); i++){
		espacioAsignado * espacio = (espacioAsignado *)list_get(listaEspacioAsignado, i);
		if (espacio->pid==pid){

			fseek(disco, (espacio->inicio + posicion) * vg_tamanio_pagina, SEEK_SET);
			fread(lectura, vg_tamanio_pagina, 1, disco);

		}
	}
	return lectura;

}


int escucharAMemoria(){	// Conexion
	int fdSwap = newSocket();
	struct sockaddr_in estructura_server = asociarSocket(fdSwap,vg_puerto_escucha);
	escucharSocket(fdSwap,1);
	int fdMemoria = aceptarConexionSocket(fdSwap);

	// Veo si quien se conecta es quien deberia conectarse.
	void * login = malloc(sizeof(char) * 4);
	recibirPorSocket(fdMemoria, login, 4);
	int coincide = strcmp((char *) login, "MEM");

	if(coincide != 0)
	{	perror("[ERROR] Se espera conexión del proceso memoria\n");
		int respuesta = 0;
		enviarPorSocket(fdMemoria,&respuesta,sizeof(int));
		cerrarSocket(fdMemoria);
		return 0;
	}
	else
	{	printf("[OK] Conexión entre Swap y Memoria establecida mediante el puerto %d\n",vg_puerto_escucha);
		int respuesta = 1;
		enviarPorSocket(fdMemoria,&respuesta,sizeof(int));
		return fdMemoria;
	}
}
