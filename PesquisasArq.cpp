#include <iostream>
#include "stdio.h"
#include "PesquisasArq.hpp"

using namespace std;
int AcessoIndexado (TabPag tab[], int tam, tipoItem* item, FILE *arq,long* leituras,long* comparacoes){ //acesso sequencial indexado
	tipoItem pagina[ITENSPAGINA];
	int i, quantitens;
	long desloc;

	// procura pela página onde o item pode se encontrar
	i = 0;
	while (i < tam && tab[i].regs.chave <= item->chave&&tab[i].regs.chave!=0){
		(*comparacoes)++;
		i++;
	}

	// caso a chave desejada seja menor que a 1a chave, o item não existe no arquivo
	if (i == 0)
		return 0;
	else {
		// a ultima página pode não estar completa
		if (i < tam)
			quantitens = ITENSPAGINA;
		else {
			fseek (arq, 0, SEEK_END);
			quantitens = (ftell(arq)/sizeof(tipoItem))%ITENSPAGINA;
			if (quantitens == 0)
				quantitens = ITENSPAGINA;
		}

		// lê a página desejada do arquivo
		desloc = (i-1)*ITENSPAGINA*sizeof(tipoItem);
		fseek (arq, desloc, SEEK_SET);
		fread (&pagina, sizeof(tipoItem), quantitens, arq);
		(*leituras)++;

		// pesquisa sequencial na página lida
		for (i=0; i < quantitens; i++){
			if (pagina[i].chave == item->chave) {
				(*comparacoes)++;
				*item = pagina[i];
				return 1;
			}
		}
		return 0;
	}
}
