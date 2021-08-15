#include "NoMapa.hpp"

#include "stdio.h"
#include "stdlib.h"

void NoMapa::addVizinho(NoMapa* vizinho) {

	NoMapa** temp = new NoMapa*[_numVizinhos+1];
	if (temp == NULL) {printf("Erro em adicao de vizinho: memoria insuficiente.\n\n");	exit(1);}

	for (int i = 0; i < _numVizinhos; i++)
		temp[i] = _vizinhos[i];
	temp[_numVizinhos] = vizinho;

	if (_vizinhos != NULL)
		delete _vizinhos;
	_vizinhos = temp;

	_numVizinhos++;
}

void NoMapa::removeVizinho(NoMapa* vizinho) {

	if (_numVizinhos < 1)
		{printf("Erro em remocao de vizinho: o no nao possui vizinhos!\n\n");	exit(1);}

	NoMapa ** temp;

	if (_numVizinhos != 1) {
		temp = new NoMapa*[_numVizinhos-1];
		if (temp == NULL) {printf("Erro em remocao de vizinho: memoria insuficiente.\n\n");	exit(1);}
	} else
		temp = NULL;

	int j = 0;

	for(int i = 0; i < _numVizinhos; i++)
		if (_vizinhos[i] != vizinho)
			if (j < _numVizinhos-1) {
				temp[j] = _vizinhos[i];
				j++;
			} else {
				printf("Erro em remocao de vizinho: vizinho inexistente.\n\n");
				exit(1);
			}

	if (_vizinhos != NULL)
		delete _vizinhos;

	_vizinhos = temp;

	_numVizinhos--;
}

NoMapa::NoMapa(int custo) {

	_numVizinhos = 0;
	_custo = custo;
	_distFinal = -1;
	_vizinhos = NULL;
	_info = NULL;
}


NoMapa::~NoMapa() {

	if (_vizinhos != NULL) {

			for (int i = 0; i < _numVizinhos; i++)
				_vizinhos[i]->removeVizinho(this);

		delete _vizinhos;
	}
}

void NoMapa::link(NoMapa* novoVizinho) {

	addVizinho(novoVizinho);
	novoVizinho->addVizinho(this);
}

void NoMapa::unlink(NoMapa* vizinho) {

	removeVizinho(vizinho);
	vizinho->removeVizinho(this);
}

int NoMapa::getCusto() {

	return _custo;
}

void NoMapa::setCusto(int custo) {

	_custo = custo;
}

int NoMapa::getDistFinal() {
	return _distFinal;
}

void NoMapa::setDistFinal(int dist) {
	_distFinal = dist;
}

void NoMapa::setInfo(void* info) {
	_info = info;
}

void* NoMapa::getInfo() {
	if (!_info) {printf("Erro: informacao requisitada inexistente.\n\n");	exit(1);}
	return _info;
}

void NoMapa::abreVizinhos(Lista &lst) {

	for (int i = 0; i < _numVizinhos; i++)
		lst.insere(0, _vizinhos[i]);
}