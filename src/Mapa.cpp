#include "Mapa.hpp"

#include "stdio.h"
#include "stdlib.h"
#include "Lista.hpp"
#include "Heap.hpp"
#include "Pos.h"

Mapa::Mapa() {

	_width = 0;
	_height = 0;
	_inicial = _pos = _final = NULL;
	_casas = NULL;
	_inicialX = _inicialY = _finalX = _finalY = 0;
}

Mapa::~Mapa() {

	if (_casas != NULL) {
		for (int i = 0; i < _height; i++)
			for (int j = 0; j < _width; j++) {
				delete (Pos*)_casas[i][j]->getInfo();
				delete _casas[i][j];
			}

		for (int i = 0; i < _height; i++) {
			delete[] _casas[i];
		}
		delete[] _casas;
	}
}

int getCusto(char custoC) {

	switch(custoC) {
	case '.':
		return 1;
	case 'G':
		return 5;
	case 'D':
		return 200;
	case 'I': case 'F': case 'C':
		return 0;
	default:
		return -1;
	}
}

void Mapa::loadMapa(char *arquivo) {

	FILE *mapa;
	mapa = fopen(arquivo, "rt");
	if (mapa == NULL) {printf("Erro (0): arquivo \"%s\" nao encontrado.\n\n", arquivo);	exit(1);}

	if (fscanf(mapa, " %d %d", &_width, &_height) != 2) {printf("Erro! arquivo \"%s\" nao esta no formato correto.\n\n", arquivo);	exit(1);}

	_casas = new NoMapa**[_height];
	if (_casas == NULL) {printf("Erro (1.0): Memoria insuficiente em criacao de matriz de ponteiros de nos.\n\n");	exit(1);}

	for (int i = 0; i < _height; i++) {
		_casas[i] = new NoMapa*[_width];
		if (_casas[i] == NULL) {printf("Erro(1.%d): Memoria insuficiente em criacao de matriz de ponteiros de nos.\n\n", i+1); exit(1);}
	}

	for (int i = 0; i < _height; i++)
		for (int j = 0; j < _width; j++) {

			char custoC;

			do {
				if (fscanf(mapa, " %c", &custoC) != 1) {printf("Erro (2): Tamanho do arquivo nao condiz com o especificado.\n\n");	exit(1);}
			} while (custoC == '\n');

			int custo = getCusto(custoC);
			if (custo == -1) {printf("Erro (3): Arquivo contem caracter desconhecido (%c).\n\n", custoC);	exit(1);}

			_casas[i][j] = new NoMapa(custo);
			Pos* indices = new Pos;
			indices->x = j;
			indices->y = i;
			_casas[i][j]->setInfo(indices);

			if (j != 0) _casas[i][j]->link(_casas[i][j-1]);
			if (i != 0) _casas[i][j]->link(_casas[i-1][j]);

			if (custo == 0) {
				if (custoC == 'I') {
					_inicial = _casas[i][j];
					_inicialX = j;
					_inicialY = i;
				} else if (custoC == 'F') {
					_final = _casas[i][j];
					_finalX = j;
					_finalY = i;
				} else if (custoC == 'C') {
					_clareiras.push(_casas[i][j]);
				}
			}
	}

	fclose(mapa);
}

void Mapa::printDistancias(char* arquivo) {

	FILE* arq;
	arq = fopen(arquivo, "wt");
	if (!arq) {printf("Erro (4): falha ao abrir arquivo \"%s\".\n\n", arquivo);	exit(1);}
	fprintf(arq, "%d %d\n", _width, _height);
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++)
			fprintf(arq, "%d\t", _casas[i][j]->getDistFinal());
		fprintf(arq, "\n");
	}
	fclose(arq);
}

int Mapa::getWidth() {
	return _width;
}
int Mapa::getHeight() {
	return _height;
}

int Mapa::getCustoNo(int i, int j) {
	if (_casas != NULL)
		return _casas[i][j]->getCusto();
	else
		{printf("Erro (4): Mapa ainda nao carregado.\n\n");	exit(1);}
}

int Mapa::getInicialX() {
	return _inicialX;
}

int Mapa::getInicialY() {
	return _inicialY;
}

int Mapa::getFinalX() {
	return _finalX;
}

int Mapa::getFinalY() {
	return _finalY;
}

Lista& Mapa::getClareiras() {
	return _clareiras;
}