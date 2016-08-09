#include "memoria.h"

void setearValores(t_config * archivoConfig){
	vg_puerto_escucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
	vg_ip_swap = strdup(config_get_string_value(archivoConfig, "IP_SWAP"));
	vg_puerto_swap = config_get_int_value(archivoConfig, "PUERTO_SWAP");
	vg_maximo_marcos_por_proceso = config_get_int_value(archivoConfig, "MAXIMO_MARCOS_POR_PROCESO");
	vg_cantidad_marcos = config_get_int_value(archivoConfig, "CANTIDAD_MARCOS");
	vg_tamanio_marco = config_get_int_value(archivoConfig, "TAMANIO_MARCO");
	vg_entradas_tlb = config_get_int_value(archivoConfig, "ENTRADAS_TLB");
	vg_tlb_habilitada = strdup(config_get_string_value(archivoConfig, "TLB_HABILITADA"));
	vg_retardo_memoria = config_get_int_value(archivoConfig, "RETARDO_MEMORIA");
}

int conectarASwap(t_log * logger){
	int retorno;

	int fdMemoria = newSocket();
	conectarSocket(fdMemoria,vg_ip_swap,vg_puerto_swap);

	char * login = strdup("MEM");
	enviarPorSocket(fdMemoria, (char*) login, 4);
	free(login);

	void * buffer = malloc(sizeof(int));
	recibirPorSocket(fdMemoria, buffer, sizeof(int));
	int numeroRecv = *((int *) buffer);
	if(numeroRecv == 1){
		printf("[OK] Conexión entre Swap y Memoria aceptada mediante el puerto %d\n",vg_puerto_swap);
		log_info(logger,"Conexión Swap - Memoria aceptada mediante el puerto %d\n",vg_puerto_swap);
		retorno = fdMemoria;
	}
	if(numeroRecv == 0){
		printf("[ERROR] Se espera conexión del proceso memoria\n");
		retorno = 0;
	}
	return retorno;
}

int escucharACPU(t_log * logger)
{
	int retorno;
	int fdMemoria = newSocket();
	asociarSocket(fdMemoria,vg_puerto_escucha);
	escucharSocket(fdMemoria,1);

	int fdCPU = aceptarConexionSocket(fdMemoria);

// Veo si quien se conecta es CPU (quien deberia conectarse).
	void * login = malloc(sizeof(char) * 4);
	recibirPorSocket(fdCPU, login, 4);
	int coincide = strcmp((char *) login, "CPU");
	free(login);

	if(coincide != 0)
	{	perror("[ERROR] Se espera conexión del proceso CPU\n");
		int respuesta = 0;
		enviarPorSocket(fdCPU,&respuesta,sizeof(int));
		cerrarSocket(fdCPU);
		retorno = 0;
	}
	else
	{	printf("[OK] Conexión entre Memoria y CPU establecida mediante el puerto: %d\n",vg_puerto_escucha);
		log_info(logger,"Conexión Memoria - CPU establecida mediante el puerto: %d\n",vg_puerto_escucha);
		int respuesta = 1;
		enviarPorSocket(fdCPU,&respuesta,sizeof(int));
		retorno = fdCPU;
	}
	return retorno;
}

int comprobarValoresBienSeteados(t_log * logger){
	int retorno = 1;
	if( isdigit(vg_cantidad_marcos) == 0){
		log_error(logger,"Archivo config de Memoria: vg_cantidad_marcos no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_entradas_tlb) == 0){
		log_error(logger,"Archivo config de Memoria: vg_entradas_tlb no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_maximo_marcos_por_proceso) == 0){
		log_error(logger,"Archivo config de Memoria: vg_maximo_marcos_por_proceso no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_puerto_escucha) == 0){
		log_error(logger,"Archivo config de Memoria: vg_puerto_escucha no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_puerto_swap) == 0){
		log_error(logger,"Archivo config de Memoria: vg_puerto_swap no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_retardo_memoria) == 0){
		log_error(logger,"Archivo config de Memoria: vg_retardo_memoria no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_tamanio_marco) == 0){
		log_error(logger,"Archivo config de Memoria: vg_tamanio_marco no es un int o esta mal configurado");
		retorno = 0;
	}
	if( vg_ip_swap == NULL){
		log_error(logger,"Archivo config de Memoria: vg_ip_swap esta mal configurado, vale NULL");
		retorno = 0;
	}
	if( vg_tlb_habilitada == NULL){
		log_error(logger,"Archivo config de Memoria: vg_tlb_habilitada esta mal configurado, vale NULL");
		retorno = 0;
	}
	return retorno;
}
