#pragma once

#include "stdlib.h"

typedef enum direc {
	NULO,
	PAI,
	ESQ,
	DIR
} Direc;

class NoHeap {
private:

	NoHeap* _pai;
	NoHeap* _esq;
	NoHeap* _dir;
	Direc _lado; // = NULO caso não possua pai
	void* _info;

public:

	NoHeap(NoHeap* pai = NULL);
	~NoHeap();

	NoHeap* inicializaPai(NoHeap* pai);

	NoHeap* getParente(Direc parente);
	NoHeap* getPai();
	NoHeap* getFilhoEsq();
	NoHeap* getFilhoDir();
	Direc getLado();

	NoHeap* trocaPai(); // Retorna o no que era pai antes da troca, ou NULL caso nao tenha havido troca
	void setInfo(void* info);
	void* getInfo();
};