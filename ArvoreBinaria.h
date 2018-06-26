#ifndef ArvoreBinaria_h
#define ArvoreBinaria_h
#include "TItem.hpp"
// struct do tipo arvore :
typedef struct 
{
	int esq;
	int dir;
	TItem item;
}TItemArvBin;

//Funções :
bool GeraArvBin (FILE*, char);
bool PesquisaArvBin (FILE*, TItem*, long*, long*);
#endif