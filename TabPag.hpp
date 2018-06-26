#ifndef TABPAG_HPP_
#define TABPAG_HPP_

#include "stdio.h"
#include "TItem.hpp"

#define ITENSPAGINA 4
#define MAXTABELA 100

typedef TItem tipoItem;

typedef struct{
	tipoItem regs;
}TabPag;

void geraTabPag(TabPag**, int, int, FILE*, long*);

#endif /* TABPAG_HPP_ */
