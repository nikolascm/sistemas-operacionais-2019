#include "curl_code.h"

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

bool searchLinksSerial(char * link, int i) {
	
	srand(time(NULL));
	static clock_t tempo;
	tempo = clock();

	CURL *curl_handle;
	static int contador = 0;
	char * page_content = download_page(curl_handle, link);
	char * real_content = strstr(page_content,"id=\"mw-content-text\"");	
	char * achou_palavra= strstr(real_content,"Santa Maria");

	if(achou_palavra!=NULL) {
		printf("\n \tExpressão encontrada!");
		printf("\n\n \tTEMPO: %f\n", (clock()-tempo)/(double)CLOCKS_PER_SEC);
		return true;
	}

	if(contador==7) {
		printf("\n\n \tTEMPO: %f\n", (clock()-tempo)/(double)CLOCKS_PER_SEC);
		return false;
	}

	contador++;
	int links_readed;
	char ** links = find_links(curl_handle, real_content, 50, &links_readed);

	// Sorteia um dos links para ser acessado.
	int random = rand() % links_readed;
	while(!verifica_palavras_invalidas(links[random]))
		random = rand() % links_readed;

	i++;
	printf("\n \tLINK %d: %s",i, links[random]);
	return searchLinksSerial(links[random],i);
    curl_global_cleanup();
}

int main(void){

	bool jogo = searchLinksSerial("https://pt.wikipedia.org/wiki/Sistema_operativo", 0);
	
	if(jogo)
		printf("\n \tVocê ganhou o jogo!\n\n");
	else
		printf("\n \tVocê perdeu o jogo.\n\n");		
	return 0;
}
