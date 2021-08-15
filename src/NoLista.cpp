#include "NoLista.hpp"

#include "stdio.h"
#include "stdlib.h"

NoLista::NoLista(NoLista* ant, NoLista* prox) {

	link(ant, this);
	link(this, prox);

	_info = NULL;
}

NoLista::~NoLista() {

	link(_ant, _prox);
}

void NoLista::link(NoLista* esq, NoLista* dir) {

	if (esq)
		esq->_prox = dir;
	if (dir)
		dir->_ant = esq;
}

void NoLista::fitInFront(NoLista* novo) {

	NoLista* velhoProx = _prox;
	link(this, novo);
	link(novo, velhoProx);
}

void NoLista::fitBehind(NoLista* novo) {

	NoLista* velhoAnt = _ant;
	link(novo, this);
	link(velhoAnt, novo);
}

NoLista* NoLista::getNext() {

	return _prox;
}

NoLista* NoLista::getPrevious() {

	return _ant;
}

void NoLista::setInfo(void* info) {

	_info = info;
}

void* NoLista::getInfo() {

	return _info;
}

void*& NoLista::getInfoRef() {

	return _info;
}