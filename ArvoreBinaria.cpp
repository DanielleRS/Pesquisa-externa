#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ArvoreBinaria.h"
using namespace std;

bool GeraArvBin (FILE* arqBin, char situacao)
{
	cout<<"GERANDO ARVORE BINARIA EM MEMORIA SECUNDARIA..."<<endl;
	TItemArvBin auxArv, temp;
	TItem auxArq;
	FILE* arqArv;
	bool atualizacao = true;
	int cont = 0;
	int j; 
	if ((arqArv=fopen("ArvBin.bin","w+b"))==NULL)
	{
		//falha ao criar o arquivo tipo Arvore Binaria
		return false;
	}
	// descobrindo o tamanho do arquivo binario :
	fseek(arqBin,0,2);
	int tam = ftell(arqBin)/sizeof(TItem);
	int i=0;
	rewind(arqBin);
	//criando a arvore binaria :
	//Caso o arquivo binario seja aleatorio temos :
	if (situacao=='3')
	{	
		while (i<tam)
		{
			if (cont == 0 ) //arvore vaiza (sem raiz)
			{
				//cria a raiz:
				fread(&auxArq, sizeof(auxArq), 1,arqBin);
				auxArv.esq = -1;
				auxArv.dir =-1;
				auxArv.item = auxArq;
				fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
				cont++;
			} else  
				{	
					//caso ja exista uma raiz :
					//1 : insere novo item
					fread(&auxArq, sizeof(auxArq), 1,arqBin);
					auxArv.esq = -1;
					auxArv.dir =-1;
					auxArv.item = auxArq;
					fseek (arqArv, cont*sizeof(TItemArvBin), 0); 
					fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
					//2 : atualiza os ponteiros
					j=0; //auxiliar para caminhar pelo arquivo da arvore atraves de fseeks
						while (atualizacao == true)
						{	
							fseek (arqArv, j*sizeof(TItemArvBin), 0);
							fread (&temp, sizeof(TItemArvBin), 1, arqArv);
							if (auxArv.item.chave > temp.item.chave) //verifico qual apontador sera atualizado (esq ou dir)
							{
								//apontador dir:
								if (temp.dir == -1) //caso o filho seja nulo, o apontador atualiza
								{
			
									atualizacao = false;
									temp.dir=cont;
									fseek (arqArv, j*sizeof(TItemArvBin), 0);
									fwrite(&temp, sizeof(TItemArvBin), 1, arqArv);
									//fim da atualizacao (a folha onde o item mais recente contigo em auxArv foi atualizada)
								} else
									{
									j= temp.dir; //nao encontramos a folha que deve ser atualizada , neste caso, continuamos seguindo
									}
							} 
							else if ( auxArv.item.chave < temp.item.chave)
									{
										//apontador esq: (funciona de forma analoga)
										if (temp.esq == -1)
										{
											atualizacao = false;
											temp.esq=cont;
											fseek (arqArv, j*sizeof(TItemArvBin), 0);
											fwrite (&temp,sizeof(TItemArvBin), 1, arqArv);
										
										} else 
											{
												j=temp.esq;//nao chegamos na folha, neste caso, pegamos o apontador o para o filho a esquerda
											}
									} else 
										break;
						}
					atualizacao = true;
					cont++; //incremento o contador
					
				}
				i++;	
		}
	} //Caso o arquivo esteja ordenado ascendentemente:
	else if (situacao=='1')
	{
		while (i<tam)
		{			
			if (cont == 0 ) //arvore vaiza (sem raiz)
			{
				//cria a raiz:
				fread(&auxArq, sizeof(auxArq), 1,arqBin);
				auxArv.esq = -1;
				auxArv.dir =-1;
				auxArv.item = auxArq;
				fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
				cont++;
			} else 
				{
					//caso ja exista uma raiz:
					//inserimos o novo item, e atualizamos seus ponteiros conforme a sequencia (ascendente)
					fread(&auxArq, sizeof(auxArq), 1,arqBin);
					auxArv.esq = -1;
					auxArv.dir =-1;
					auxArv.item = auxArq;
					fseek (arqArv, cont*sizeof(TItemArvBin), 0); 
					fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
					//uma vez inserido o item, atualizamos o ponteiro anterior :
					fseek (arqArv,  (cont-1)*sizeof(TItemArvBin), 0); //voltamos para o item anterior ao inserido
					fread (&auxArv, sizeof (TItemArvBin),1, arqArv);
					auxArv.dir=cont; // realiza a atualizacao
					fseek (arqArv, (cont-1)*sizeof(TItemArvBin),0);
					fwrite (&auxArv, sizeof (TItemArvBin),1,arqArv); //grava a atualizacao
					cont++;
					
					
				}
			i++;
		}
	}//Caso o arquivo binario seja ordenado descendentemente:
	else if (situacao=='2')
	{
		//Funciona de forma analoga a anterior, mas os ponteiros a serem atualizados são os da esquerda
		while (i<tam)
		{			
			if (cont == 0 ) //arvore vaiza (sem raiz)
			{
				//cria a raiz:
				fread(&auxArq, sizeof(auxArq), 1,arqBin);
				auxArv.esq = -1;
				auxArv.dir =-1;
				auxArv.item = auxArq;
				fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
				cont++;
			} else 
				{
					//caso ja exista uma raiz:
					//insere :
					fread(&auxArq, sizeof(auxArq), 1,arqBin);
					auxArv.esq = -1;
					auxArv.dir =-1;
					auxArv.item = auxArq;
					fseek (arqArv, cont*sizeof(TItemArvBin), 0); 
					fwrite(&auxArv, sizeof(TItemArvBin), 1, arqArv);
					//atualiza o ponteiro anterior :
					fseek (arqArv,  (cont-1)*sizeof(TItemArvBin), 0); //voltamos para o item anterior ao inserido
					fread (&auxArv, sizeof (TItemArvBin),1, arqArv);
					auxArv.esq=cont; // realiza a atualizacao
					fseek (arqArv, (cont-1)*sizeof(TItemArvBin),0);
					fwrite (&auxArv, sizeof (TItemArvBin),1,arqArv); //grava a atualizacao
					cont++;
					
					
				}
			i++;	
		}
	}	
	rewind (arqBin);	
	fclose (arqArv);
	return true;
}

bool PesquisaArvBin(FILE* arqArv, TItem* x, long* leituras, long* comparacoes)
{
	cout<<"PESQUISANDO NA ARVORE BINARIA SECUNDARIA..."<<endl;
	TItemArvBin aux;
	bool encontrou = false;
	if ((arqArv = fopen("ArvBin.bin","rb")) == NULL)
	{
		//falha ao abrir a arvore 
		return false;
	}
	int j=0; //auxiliar para caminhar no arquivo
	while (true)
	{
		(*comparacoes)++;
		//inicia a busca :
		fseek (arqArv, j*sizeof(TItemArvBin), 0);
		fread(&aux, sizeof(TItemArvBin), 1, arqArv);
		(*leituras)++;
		if (x->chave == aux.item.chave) //caso seja a chave:
			{
				(*comparacoes)++;
				*x = aux.item;
				encontrou = true;
				break;
			} else if (x->chave > aux.item.chave) // caso seja maior (filho a direita) :
					{
						(*comparacoes)++;
						
						//verificamos se existe um filho a direita, e caso exista, direcionamos a busca prara ele
						(*comparacoes)++; //if abaixo:
						if (aux.dir == -1)
						{
							break; //neste caso, nao existe a chave
						
						}else // direcionando :
							{
								j = aux.dir;
							}
					}else  //caso seja menor, o processo se repete de forma analoga para o filho a esquerda: 
						{
							(*comparacoes)++;//if abaixo:
							if (aux.esq == -1)
							{
								break;
							} else 
								{
									j=aux.esq;
								}
						}
	}
	fclose(arqArv);
	return encontrou;
}