#include "planificador.h"

void setearValores(t_config * archivoConfig){
	vg_puerto_escucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
	vg_quantum = config_get_int_value(archivoConfig, "QUANTUM");
	vg_algoritmo_planificacion = strdup(config_get_string_value(archivoConfig, "ALGORITMO_PLANIFICACION"));

}

int testVGBienSeteadas(t_log * logger){
	int retorno = 1;
	if( isdigit(vg_puerto_escucha) == 0){
		log_error(logger,"setearValores() - vg_puerto_escucha no es un int o esta mal seteado");
		retorno = 0;
	}
	if( isdigit(vg_quantum) == 0){
		log_error(logger,"setearValores() - vg_quantum no es un int o esta mal seteado");
		retorno = 0;
	}
	if(vg_algoritmo_planificacion == NULL){
		log_error(logger,"setearValores() - vg_algoritmo_planificacion vale NULL");
		retorno = 0;
	}
	return retorno;
}

void leerComandos(t_list * listaProcesos, t_list * listaCPU){


	printf("Escriba los comandos:\n");

	char entrada[100];

	fgets(entrada, sizeof(entrada), stdin);

	while(entrada[0]!='\n'){
		entrada[strlen(entrada)-1]='\0';
		char** comando = string_split(entrada, " ");

		if (strcmp("correr", comando[0])==0){
			printf("CORRIENDO PROG %s\n", comando[1]);

			pcb * nuevoPcb = malloc(sizeof(pcb));

			nuevoPcb->id= crearNumId(listaProcesos);
			nuevoPcb->cpu=1;
			nuevoPcb->inicio=1;
			if (strcmp("RR", vg_algoritmo_planificacion)==0){
				nuevoPcb->cantidad=vg_quantum;
			}else{
				nuevoPcb->cantidad=0;
			}

			char cwd[150];
			getcwd(cwd, sizeof(cwd));
			strcpy(nuevoPcb->ruta, cwd);
			strcat(nuevoPcb->ruta, "/");
			strcat(nuevoPcb->ruta, comando[1]);

			list_add(listaProcesos, nuevoPcb);

			int i;
			for (i = 0; i < list_size(listaCPU); i++){
				cpu * unCPU = (cpu *)list_get(listaCPU, i);
				enviarPaquete(unCPU->fd, 1, nuevoPcb, sizeof(pcb));
			}

		}else if (strcmp("finalizar", comando[0])==0){
			printf("FINALIZANDO PROG %s\n", comando[1]);
		}else if (strcmp("ps", comando[0])==0){
			printf("PROCESOS EN EJECUCION: \n");
			int i;
			for (i = 0; i < list_size(listaProcesos); i++){
				pcb * unPcb = (pcb *)list_get(listaProcesos, i);
				printf("%i %s\n", unPcb->id, unPcb->ruta);
			}
		}else if (strcmp("cpu", comando[0])==0){
			//printf("CARGA DEL CPU: 0 %% \n");
			usoCpu(listaCPU, listaProcesos);
		}else{
			printf("[ERROR] Comando invalido\n");
		}
		free(comando);

		fgets(entrada, sizeof(entrada), stdin);
	}
}

void escucharACPU(t_list * listaCPU){	// Conexion

	int fdPlanificador, fdNuevoCPU, maxConexiones = 10, i, sd;
	int max_sd;

	//set of socket descriptors
	fd_set readfds;


	fdPlanificador = newSocket();

	asociarSocket(fdPlanificador, vg_puerto_escucha);

	escucharSocket(fdPlanificador, maxConexiones);

	while(1){
		//clear the socket set
	    FD_ZERO(&readfds);

	    //add master socket to set
	    FD_SET(fdPlanificador, &readfds);
	    max_sd = fdPlanificador;

	    //add child sockets to set
	    for ( i = 0 ; i < list_size(listaCPU) ; i++){

	    	cpu * unCPU = (cpu *)list_get(listaCPU, i);

	        //socket descriptor
	        sd = unCPU->fd;

	        //if valid socket descriptor then add to read list
	        if(sd > 0)
	            FD_SET( sd , &readfds);

	        //highest file descriptor number, need it for the select function
	        if(sd > max_sd)
	            max_sd = sd;
	    }

	    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
	    //select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	    seleccionarSocket(max_sd, &readfds , NULL , NULL , NULL, NULL);


	    //If something happened on the master socket , then its an incoming connection
	    if (FD_ISSET(fdPlanificador, &readfds)) {
	    	fdNuevoCPU = aceptarConexionSocket(fdPlanificador);

	    	// Veo si quien se conecta es quien deberia conectarse.
	    	void * login = malloc(sizeof(char) * 4);
	    	recibirPorSocket(fdNuevoCPU, login, 4);
	    	int coincide = strcmp((char *) login, "CPU");

	    	if(coincide != 0){
	    		perror("[ERROR] Se espera conexión del proceso CPU\n");
	    		int respuesta = 0;
	    		enviarPorSocket(fdNuevoCPU,&respuesta,sizeof(int));
	    		cerrarSocket(fdNuevoCPU);
	    	}else{
	    		int respuesta = 1;
	    		enviarPorSocket(fdNuevoCPU,&respuesta,sizeof(int));

	    		cpu * nuevoCPU = malloc(sizeof(cpu));

	    		nuevoCPU->id= fdNuevoCPU - fdPlanificador;
	    		nuevoCPU->fd = fdNuevoCPU;

	    		list_add(listaCPU, nuevoCPU);

	    		printf("[OK] Conexión entre Planificador y CPU %i establecida mediante el puerto: %d\n", fdNuevoCPU - fdPlanificador,vg_puerto_escucha);
	        }
	    }
	    //else its some IO operation on some other socket :)
	    for ( i = 0 ; i < list_size(listaCPU) ; i++){

	    	cpu * unCPU = (cpu *)list_get(listaCPU, i);

	    	//socket descriptor
	    	sd = unCPU->fd;

	        if (FD_ISSET(sd , &readfds)) {
	            int tipoMensaje;
	            int tamanioMensaje;
	            void * mensaje = recibirPaquete(sd, &tipoMensaje, &tamanioMensaje);
	            if (mensaje == NULL){
	            	cerrarSocket(sd);
	            	list_remove(listaCPU, i);
	            	printf("SE DESCONECTO CPU %i\n", unCPU->id);

	            }else{
	            	//RECIBI UN MENSAJE DE CPU
	                printf("RECIBI ALGO DE CPU\n");
	            }
	        }
	    }
	}

}

int crearNumId(t_list * unaLista){
	int id = rand() % 10000;
	while ( perteneceId(unaLista,id) ){
		id = rand() % 10000;
	}
	return id;
}


int perteneceId(t_list * unaLista, int unId){
	int pertenece = 0;
	int i;

	for(i=0; list_size(unaLista) != i ; i++){
		pcb * unPcb = (pcb *) list_get(unaLista,i);
		(unPcb->id == unId)? (pertenece = 1) : (pertenece = 0);
		if(pertenece == 1) break;
	}
	return pertenece;
}

void usoCpu(t_list* listaCpu, t_list* listaPcb){
	int procesosTot = list_size(listaPcb);
	int i,j;
	int procesosAsocACpu = 0;
	float usoCpu;

	for(i=0; i<list_size(listaCpu); i++){

		cpu* unCpu = (cpu *) list_get(listaCpu,i);

		for(j=0; j<procesosTot; j++){
			pcb* unPcb = (pcb *) list_get(listaPcb,j);

			if(unPcb->cpu == unCpu->id){
				procesosAsocACpu++;
			}

		}
		usoCpu = (procesosAsocACpu*100)/procesosTot;
		printf("CPU %d : %f %%\n",unCpu->id,usoCpu);
	}

}
