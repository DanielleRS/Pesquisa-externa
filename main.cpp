#include <iostream>
#include <iomanip>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "PesquisasArq.hpp"
#include "geraArqs.hpp"
#include "ArvoreBinaria.h"
#include "ArvoreB.h"
using namespace std;

int main(int argc, char* argv[]){
	FILE* arq1;
	FILE* arq2;
	TabPag* tabela;
	TItem x;

	short metodo;
	int quantidade;
	char situacao;
	int chave;

	clock_t c1, c2;
	double tempo;

	long leituras, comparacoes;
	leituras=comparacoes=0;

	if(argc == 5 || argc == 6){
		metodo = atoi(argv[1]);
		quantidade = atoi(argv[2]);
		situacao = argv[3][0];
		chave = atoi(argv[4]);
	}
	else{
		cout << "FORMATO DE EXECUÇÃO: pesquisa <método> <quantidade> <situação> <chave> [-P]\n";
		exit(1);
	}

	cout << "GERANDO ARQUIVO DE DADOS...\n";
	geraArqs(quantidade,situacao);
	if((arq1=fopen("dados.bin","r+b"))==NULL){
		puts ("Arquivo nao pode ser aberto...");
		exit (1);
	}
	if(argc == 6 && argv[5][1] =='P'){
		while(fread(&x,sizeof(TItem),1,arq1)==1)
			cout << x.chave << "\n";
		rewind(arq1);
	}
		/*CONSIDERAÇOES:
		 * PARA AUTOMATIZAR OS TESTES, NA LINHA DE EXECUÇÃO, DEIXE A CHAVE DE PESQUISA COMO -1
		 * SERÃO REALIZADOS 10 PESQUISAS COM CHAVES ALEATORIAS, SENDO QUE 5 CERTAMENTE SERÃO ENCONTRADAS E 5 PODEM OU NÃO SER ENCONTRADAS
		 * A MEDIA DOS VALORES DE ANALISE JÁ É CALCULADA
		 */
	//METODOS : (DE 1 A 4)
	if (metodo == 1) {
		//ACESSO SEQUENCIAL INDEXADO :
		// gera a tabela de índice das páginas
		geraTabPag(&tabela,quantidade/ITENSPAGINA,ITENSPAGINA,arq1,&leituras);
		if (chave == -1) { //BUSCA AUTOMATICA:
			srand(0);
			for (int i = 0; i < 10; i++) {
				if(i%2){
					fseek(arq1,0,2);
					int desloc = (-1)*(rand()%(quantidade)+1);
					fseek(arq1,desloc*sizeof(TItem),2);
					fread(&x,sizeof(tipoItem),1,arq1);
					cout << "CHAVE A SER BUSCADA: " <<  x.chave <<endl;
				}
				else{
					x.chave = rand();
					cout << "CHAVE A SER BUSCADA: " <<  x.chave <<endl;
				}
				rewind(arq1);

				c1 = clock();
				if (AcessoIndexado(tabela, quantidade/ITENSPAGINA, &x, arq1,&leituras,&comparacoes))
					printf ("Cahve %d localizada.\nDado1 - %li \nDado2 - %s\n", x.chave,x.dado1,x.dado2);
				else
					printf ("Chave de código %d não localizada\n",x.chave);
				c2 = clock();
				tempo += ((c2 - c1)*1.0)/CLOCKS_PER_SEC;
				cout << endl;
			}
			tempo/=10.0;
			comparacoes/=10.0;
			leituras/=10.0;
		}
		else { //BUSCA MANUAL:
			x.chave = chave;
			c1 = clock();
			if (AcessoIndexado (tabela, quantidade/ITENSPAGINA, &x, arq1,&leituras,&comparacoes)) //PESQUISANDO SEMPRE A ULTIMA CHAVE SOMENTE PARA TESTE
				printf ("Cahve %d localizada.\nDado1 - %li \nDado2 - : %s\n", x.chave,x.dado1,x.dado2);
			else
				printf ("Chave de código %d não localizada\n",x.chave);
			c2 = clock();
			tempo = ((c2 - c1)*1.0)/CLOCKS_PER_SEC;
			cout << endl;
		}
			delete [] tabela;
	}

	else if(metodo == 2){
		//ARVORE BINARIA EXTERNA
		if (chave == -1) { //BUSCA AUTOMATICA:
			GeraArvBin (arq1,situacao); //GERA A ARVORE EM MEMORIA EXTERNA
			srand(0);
			for (int i = 0; i < 10; i++) {
				if(i%2){
					fseek(arq1,0,2);
					int desloc = (-1)*(rand()%(quantidade)+1);
					fseek(arq1,desloc*sizeof(TItem),2);
					fread(&x,sizeof(tipoItem),1,arq1);
					cout << "CHAVE A SER BUSCADA: " <<  x.chave <<endl;
				}
				else{
					x.chave = rand();
					cout << "CHAVE A SER BUSCADA: " <<  x.chave <<endl;
				}
				rewind(arq1);

				c1 = clock();
				if (PesquisaArvBin (arq1,&x,&leituras,&comparacoes))
					printf ("Cahve %d localizada.\nDado1 - %li \nDado2 - %s\n", x.chave,x.dado1,x.dado2);
				else
					printf ("Chave de codigo %d nao localizada\n",x.chave);
				c2 = clock();
				tempo += ((c2 - c1)*1.0)/CLOCKS_PER_SEC;
				cout << endl;
			}
			tempo/=10.0;
			comparacoes/=10.0;
			leituras/=10.0;
		}
		else { //BUSCA MANUAL :
			x.chave = chave;
			GeraArvBin (arq1, situacao); // GERA A ARVORE EM MEMORIA EXTERNA
			c1 = clock();
			if (PesquisaArvBin(arq1, &x, &leituras, &comparacoes)) //PESQUISANDO SEMPRE A ULTIMA CHAVE SOMENTE PARA TESTE
				printf ("Cahve %d localizada.\nDado1 - %li \nDado2 - : %s\n", x.chave,x.dado1,x.dado2);
			else
				printf ("Chave de codigo %d nao localizada\n",x.chave);
			c2 = clock();
			tempo = ((c2 - c1)*1.0)/CLOCKS_PER_SEC;
			cout << endl;
		}

	}

	else if(metodo == 3){
		//ARVORE B
		TipoApontador Ap;
		TipoRegistro Reg;
		int resp=0; double tempo1;
		Inicializa(&Ap);
		cout << endl;
		if (chave == -1) { //Busca automatica
			srand(0);
			c1 = clock();
			// Lendo do arquivo e inserindo na arvore b
			for(int i=0; i<quantidade; i++){
				fread(&Reg,sizeof(TItem),1,arq1);
				Insere(Reg, &Ap);
			}
			c2 = clock();
			tempo1 = (1.0*(c2 - c1)) / CLOCKS_PER_SEC;

			for (int i = 0; i < 10; i++) {
				if(i%2){
					fseek(arq1,0,2);
					int desloc = (-1)*(rand()%(quantidade)+1);
					fseek(arq1,desloc*sizeof(TItem),2);
					fread(&Reg,sizeof(tipoItem),1,arq1);
					cout << "CHAVE A SER BUSCADA: " <<  Reg.Chave << endl;
				}
				else{
					Reg.Chave = rand();
					cout << "CHAVE A SER BUSCADA: " <<  Reg.Chave << endl;
				}
				rewind(arq1);

				c1 = clock();
				Pesquisa(&Reg,Ap,&comparacoes,&resp);
				c2 = clock();
				tempo += (1.0*(c2 - c1)) / CLOCKS_PER_SEC;
				if(resp == 1){
					cout << "CHAVE: " << Reg.Chave << endl;
					cout << "DADO1: " << Reg.dado1 << endl;
					cout << "DADO2: " << Reg.dado2 << endl;
					cout << endl;
				}
				else if(resp == 0)
					cout << "Registro nao encontrado." << endl;
					cout << endl;
			}
			leituras = quantidade;
			tempo /= 10.0;
			tempo += tempo1;
			comparacoes /= 10.0;
			cout << endl;
		}
		else{ // Busca manual
			c1 = clock();
			for(int i=0; i<quantidade; i++){
				fread(&Reg,sizeof(TItem),1,arq1);
				Insere(Reg, &Ap);
			}
			Reg.Chave = chave;
			Pesquisa(&Reg,Ap,&comparacoes,&resp);
			c2 = clock();
			tempo = (1.0*(c2 - c1)) / CLOCKS_PER_SEC;

			if(resp == 1){
				cout << "CHAVE: " << Reg.Chave << endl;
				cout << "DADO1: " << Reg.dado1 << endl;
				cout << "DADO2: " << Reg.dado2 << endl;
			}
			else if(resp == 0)
				cout << "Registro nao encontrado." << endl;
		}
		leituras = quantidade;
	}
	else if(metodo == 4){
        cout << "METODO NAO IMPLEMENTADO " << endl;
        exit(1);
	}
	cout << endl;
	cout << "TEMPO DE EXECUCAO: " << fixed << tempo << " segundos" << endl;
	cout << "LEITURAS: " << leituras << endl;
	cout << "COMPARACOES: " << comparacoes << endl;

	fclose(arq1);
	return 0;
}
