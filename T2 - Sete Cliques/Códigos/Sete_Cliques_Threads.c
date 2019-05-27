#include "curl_code.h"

typedef struct { 
	char * link; 
	int posVetor;
	int contador;
} Argumentos; 

bool verifica_palavras_invalidas(char * string) {

	char ** palavras_invalidas = (char**) malloc(NUM_WORDS*sizeof(char*));
	palavras_invalidas[0] = "/Wikipédia";
	palavras_invalidas[1] = "/Categoria";
	palavras_invalidas[2] = "/Especial";
	palavras_invalidas[3] = "/Portal";
	palavras_invalidas[4] = "/Ajuda";
	palavras_invalidas[5] = "Ficheiro";
	palavras_invalidas[6] = "desambiguação";
	palavras_invalidas[7] = "Carregar_ficheiro";

	for(int i=0; i<NUM_WORDS; i++) {
		if(strstr(string,palavras_invalidas[i])!=NULL)
			return false;
	}
	return true;
}

void * searchLinksThreads(void * args) {
	
	srand(time(NULL));
	static clock_t tempo;
	tempo = clock();
	
	CURL *curl_handle;
	Argumentos * in = (Argumentos *) args;

	char * page_content = download_page(curl_handle, in->link);
	char * real_content = strstr(page_content,"id=\"mw-content-text\"");	
	char * achou_palavra= strstr(real_content,"Santa Maria");

	if(achou_palavra!=NULL) {
		printf("\n\n \tTHREAD %d encontrou a expressão!\n \tTEMPO: %f\n", in->posVetor+1, (clock()-tempo)/(double)CLOCKS_PER_SEC);
		pthread_exit((void *)true);
	}

	if(in->contador==7) {
		printf("\n\n \tTHREAD %d executou 7 vezes!\n \tTEMPO: %f\n", in->posVetor+1, (clock()-tempo)/(double)CLOCKS_PER_SEC);
		pthread_exit((void *)false);
	}

	in->contador++;
	int links_readed;
	char ** links = find_links(curl_handle, real_content, 50, &links_readed);

	// Sorteia um dos links para ser acessado.
	int random = rand() % links_readed;
	while(!verifica_palavras_invalidas(links[random]))
		random = rand() % links_readed;

	printf("\n \tThread %d -> LINK %d: %s",in->posVetor+1, in->contador, links[random]);
	
	Argumentos arg;
	arg.link = links[random];
	arg.posVetor = in->posVetor;
	arg.contador = in->contador;
	searchLinksThreads((void*)&arg);
}

bool inicializa_Threads(char * argv) {

	bool * retorno;
  	int NUM_THREADS;
	int statusThread;
	void * retornoThread;
	pthread_t * threads;

	Argumentos * args;
	
	if (argv == NULL)
		return false;
	else {
		NUM_THREADS = atoi(argv);
		threads = (pthread_t *) malloc(NUM_THREADS*sizeof(pthread_t));
		args = (Argumentos *) malloc(NUM_THREADS*sizeof(Argumentos));
		retorno = (bool *) malloc(NUM_THREADS*sizeof(bool));
		printf("\n");
	}
	
	for(int i=0; i<NUM_THREADS; i++) {
		args[i].posVetor = i;
		args[i].contador = 0;
		args[i].link = "https://pt.wikipedia.org/wiki/Sistema_operativo";
		
		statusThread = pthread_create(&threads[i], NULL, searchLinksThreads, (void*)&args[i]);
		if(statusThread != 0) {
 			printf ("Erro ao criar Thread %d.\n", i);
 			exit(EXIT_FAILURE);
		} else {
			printf("\tThread %d inicializada\n", i+1);
		}
	}

	for(int i=0; i<NUM_THREADS; i++) {
		statusThread = pthread_join(threads[i], &retornoThread);
		retorno[i] = (bool)retornoThread;
	}

	for(int i=0; i<NUM_THREADS; i++) {
		if(retorno[i] == true)
			return true;
	}
	curl_global_cleanup();
	return false;
}

int main(int argc, char**argv){
	
	bool jogo = inicializa_Threads(argv[1]);
	
	if(jogo)
		printf("\n\n \tVocê ganhou o jogo!\n\n");
	else
		printf("\n\n \tVocê perdeu o jogo.\n\n");			
	return 0;
}
