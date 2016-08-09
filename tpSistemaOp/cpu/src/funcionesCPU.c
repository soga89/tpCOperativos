#include "cpu.h"

void setearValores(t_config * archivoConfig){
	vg_puerto_memoria = config_get_int_value(archivoConfig, "PUERTO_MEMORIA");
	vg_puerto_planificador = config_get_int_value(archivoConfig, "PUERTO_PLANIFICADOR");
	vg_cantidad_hilos = config_get_int_value(archivoConfig, "CANTIDAD_HILOS");
	vg_retardo = config_get_int_value(archivoConfig, "RETARDO");
	vg_ip_memoria = strdup(config_get_string_value(archivoConfig, "IP_MEMORIA"));
	vg_ip_planificador = strdup(config_get_string_value(archivoConfig, "IP_PLANIFICADOR"));
}

int conectarAPlanificador(t_log * logger){
	int retorno;
	int fdPlanificador = newSocket();
	conectarSocket(fdPlanificador,vg_ip_planificador,vg_puerto_planificador);

	char * login = strdup("CPU");
	enviarPorSocket(fdPlanificador, (char*) login, 4);

	void * buffer = malloc(sizeof(int));
	recibirPorSocket(fdPlanificador, buffer, sizeof(int));
	int numeroRecv = *((int *) buffer);
	if(numeroRecv == 1){
		printf("[OK] Conexion CPU - Planificador establecida mediante el puerto %d \n",vg_puerto_planificador);
		log_info(logger,"Conexion CPU - Planificador establecida mediante el puerto %d \n",vg_puerto_planificador);
		retorno = fdPlanificador;
	}
	if(numeroRecv == 0){
		printf("[ERROR] Se espera conexión del proceso CPU\n");
		retorno = 0;
	}
	return retorno;
}

int conectarAMemoria(t_log * logger){
	int retorno;
	int fdMemoria = newSocket();
	conectarSocket(fdMemoria,vg_ip_memoria,vg_puerto_memoria);

	char * login = strdup("CPU");
	enviarPorSocket(fdMemoria, (char*) login, 4);

	void * buffer = malloc(sizeof(int));
	recibirPorSocket(fdMemoria, buffer, sizeof(int));
	int numeroRecv = *((int *) buffer);
	if(numeroRecv == 1){
		printf("[OK] Conexion CPU - Memoria establecida mediante el puerto %d \n",vg_puerto_memoria);
		log_info(logger,"Conexion CPU - Memoria establecida mediante el puerto %d",vg_puerto_memoria);

		retorno = fdMemoria;
	}
	if(numeroRecv == 0){
		printf("[ERROR] Se espera conexión del proceso Memoria\n");
		retorno = 0;
	}
	return retorno;
}

int compruebaDatosBienSeteados(t_log * logger){
	int retorno = 1;
	if( isdigit(vg_cantidad_hilos) == 0){
		log_error(logger,"Archivo config de CPU: La cantidad de hilos no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_puerto_memoria)  == 0){
		log_error(logger,"Archivo config de CPU: El puerto de memoria no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_puerto_planificador) == 0 ){
		log_error(logger,"Archivo config de CPU: El puerto de planificador no es un int o esta mal configurado");
		retorno = 0;
	}
	if( isdigit(vg_retardo)  == 0){
		log_error(logger,"Archivo config de CPU: Retardo no es un int o esta mal configurado");
		retorno = 0;
	}
	if( vg_ip_memoria == NULL){
		log_error(logger,"Archivo config de CPU: Mal seteado el valor de ip de la memoria");
		retorno = 0;
	}
	if( vg_ip_planificador == NULL){
		log_error(logger,"Archivo config de CPU: Mal seteado el valor de ip del planificador");
		retorno = 0;
	}
	return retorno;
}

