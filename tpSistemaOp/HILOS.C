
typedef struct nodo_hilo{
	pthread_t hilo;
	struct nodo_hilo * siguiente;
} hilo;



void agregarHilo(pthread_t id_hilo){

	hilo * nuevo_hilo = NULL;

	nuevo_hilo = (hilo *) malloc(sizeof(hilo));

	nuevo_hilo->hilo = id_hilo;
	nuevo_hilo->siguiente = NULL;

	nuevo_hilo->siguiente = vg_hilos;
	vg_hilos = nuevo_hilo;
}

/**
* @NAME: eliminarListaDeHilos
* @DESC: Elimina toda la lista de hilos del administrador de memoria
* @PARAMS:
*
*/
void eliminarListaDeHilos(){

	hilo * hilo = NULL;

	for(hilo = vg_hilos; vg_hilos ; vg_hilos = hilo->siguiente, free(hilo), hilo = vg_hilos);
}

/**
* @NAME: esperarHilos
* @DESC: Realiza un pthread_join por cada hilo creado en el administrador de memoria
* @PARAMS:
*
*/
void esperarHilos(){

	hilo * hilo = NULL;

	for(hilo = vg_hilos ; hilo ; pthread_join(hilo->hilo, NULL), hilo = hilo->siguiente);

	if(vg_tlb_habilitada) pthread_cancel(vg_hilo_escritura);
}