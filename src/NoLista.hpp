#pragma once

#include "stdlib.h"

class NoLista {
private:

	NoLista* _ant;
	NoLista* _prox;
	void* _info;

	void link(NoLista* esq, NoLista* dir);

public:

	NoLista(NoLista* ant = NULL, NoLista* prox = NULL);
	~NoLista();

	void fitInFront(NoLista* novo);
	void fitBehind(NoLista* novo);

	NoLista* getNext();
	NoLista* getPrevious();

	void setInfo(void* info);
	void* getInfo();
	void*& getInfoRef();
};