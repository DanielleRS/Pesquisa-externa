#include "stdio.h"
#include "stdlib.h"
#include <string>
#include "TItem.hpp"
#include "time.h"

std::string geraRandString(){
	std::string retorno;
	int tam_string = rand()%499 + 1;
	char * temp = new char[tam_string];
	char alfabeto[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', ' '};

	for(int i=0;i<tam_string;i++)
		temp[i] = alfabeto[rand()%sizeof(alfabeto)/sizeof(char)];

	retorno = temp;
	delete [] temp;
	return retorno;
}

void geraArqs(int num_regs, char situacao){
	int* aleatorios = new int [num_regs];
	FILE* arq;
	std::string temp;
	TItem* x = new TItem[num_regs];

	srand(time(0));

	if ((arq = fopen("dados.bin","w+b")) == NULL){
		puts ("Arquivo nao pode ser aberto...");
		exit (1);
	}

	if(situacao=='1'){
		for (int j = 0; j < num_regs; j++) {
			temp = geraRandString();
			for(int i=0;i<temp.length()-1;i++)
				x[j].dado2[i]=temp[i];
			temp.clear();
			x[j].dado1 = rand();

			x[j].chave = j+1;
		}
		fwrite(&x[0],sizeof(TItem),num_regs,arq);
	}

	else if(situacao=='2'){
		for (int j = 0; j < num_regs; j++) {
			temp = geraRandString();
			for(int i=0;i<temp.length()-1;i++)
				x[j].dado2[i]=temp[i];
			temp.clear();

			x[j].dado1 = rand();
			x[j].chave = num_regs-j;

		}
		fwrite(&x[0],sizeof(TItem),num_regs,arq);
	}

	else if(situacao=='3'){
		char *vet = new char [num_regs];
		int pos;
		for(int i=0;i<num_regs;i++){
			vet[i]='n';
		}
		for (int j = 0; j < num_regs; j++) {
			temp = geraRandString();
			for(int i=0;i<temp.length()-1;i++)
				x[j].dado2[i]=temp[i];
			temp.clear();
			pos = rand()%num_regs;
			while(true){
				if(vet[pos]=='n'){
					x[j].chave=pos+1;
					vet[pos]='s';
					break;
				}
				else pos = rand()%num_regs;
			}

			x[j].dado1 = rand();
		}
		fwrite(&x[0],sizeof(TItem),num_regs,arq);
	}
	fclose(arq);
	delete [] aleatorios;
}
