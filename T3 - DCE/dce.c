#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

void * bolsista(void * args) {
    printf("\nEntrei\n");

}

void * alunos(void * arg) {

    int * ficha = (int *)arg;
    printf("\nFicha %d: Entrei. Trouxe meus documentos.", *ficha);
    printf("\nFicha %d: Vou esperar pela minha carteirinha...", *ficha);

}


void inicializa_threads(int num_alunos) {

	int statusThread;

    pthread_t * threads_Alunos;
	threads_Alunos = (pthread_t *) malloc(num_alunos*sizeof(pthread_t));

	for(int i=0; i<num_alunos; i++) {
		statusThread = pthread_create(&threads_Alunos[i], NULL, alunos, &i);
		if(statusThread != 0) {
 			printf ("Erro ao criar Thread %d.\n", i);
 			exit(EXIT_FAILURE);
		}
	}

    for(int i=0; i<num_alunos; i++)
		statusThread = pthread_join(threads_Alunos[i], NULL);
/*
    pthread_t thread_Bolsista;
    pthread_create(&thread_Bolsista, NULL, bolsista, NULL);
    pthread_join(thread_Bolsista, NULL);
    */
}


int main(int argc, char*argv[]) {

    int num_alunos = 0;
    if(argv[1] == NULL)
        exit(0);
    else
        num_alunos = atoi(argv[1]);
    
    printf("Número de alunos (fichas): %d\n\n", num_alunos);

    inicializa_threads(num_alunos);
    return 0;
}