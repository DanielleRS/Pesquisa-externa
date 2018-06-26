#include "TabPag.hpp"
#include <iostream>

void geraTabPag(TabPag** tab, int tam_tab, int tam_pag, FILE* arq, long* leituras){
	(*tab) = new TabPag [tam_tab];
	for(int i=0;i<tam_tab;i++){
		(*tab)[i].regs.chave=0;
	}
	int pos=0;
	long desloc;
	tipoItem x;

	desloc=sizeof(tipoItem)*(tam_pag-1);
	while(fread(&x, sizeof(tipoItem), 1, arq)==1){
		(*leituras)++;
		fseek(arq,desloc,1);
		(*tab)[pos++].regs.chave = x.chave;
	}
}
