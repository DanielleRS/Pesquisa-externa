#include <iostream>
#include <stdlib.h>
#define MM 4
#define M 2

using namespace std;

typedef long TipoChave;

typedef struct TipoRegistro {
	TipoChave Chave;
	long dado1;
	char dado2[501];
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
	short n;
	TipoRegistro r[MM];
	TipoApontador p[MM + 1];
} TipoPagina;

void Inicializa(TipoApontador*);
void Pesquisa(TipoRegistro *, TipoApontador, long *, int *);
void InsereNaPagina(TipoApontador, TipoRegistro, TipoApontador);
void Ins(TipoRegistro, TipoApontador, short *, TipoRegistro *, TipoApontador *);
void Insere(TipoRegistro, TipoApontador *);
