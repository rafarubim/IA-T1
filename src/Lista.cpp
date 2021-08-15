#include "Lista.hpp"

#include "stdio.h"
#include "stdlib.h"

Lista::Lista() {

	_prim = _ult = NULL;
	_numElem = 0;
}

Lista::~Lista() {

	while (_prim) {

		NoLista* temp = _prim;
		_prim = _prim->getNext();
		delete temp;
	}
}

NoLista* Lista::jumpToPos(int pos) {

	if (pos < 0 || pos >= _numElem)
		{printf("Erro (1) em modulo \"Lista.cpp\": impossivel saltar para posicao desejada.\n\n");	exit(1);}

	NoLista* scan = _prim;

	while(pos--)
		scan = scan->getNext();

	return scan;
}

bool Lista::vazia() {

	if (!_prim)
		return 1;
	else
		return 0;
}

void Lista::push(void* info) {

	NoLista* novo = new NoLista;
	novo->setInfo(info);
	if (_ult)
		_ult->fitInFront(novo);
	else
		_prim = _ult = novo;
	_numElem++;
	_ult = novo;
}

void Lista::insere(int pos, void* info) {

	if (pos < 0 || pos > _numElem) {
		printf("Aviso (1) em modulo \"Lista.cpp\": posicao de insercao invalida. Nenhum elemento fo inserido.\n");
		return;
	}

	if (pos == 0) {

		if (_numElem == 0)
			_prim = _ult = new NoLista;
		else {
			NoLista* novo = new NoLista;
			_prim->fitBehind(novo);
			_prim = novo;
		}
		_prim->setInfo(info);
		_numElem++;
		return;
	}

	NoLista* temp = jumpToPos(pos-1);

	NoLista* novo = new NoLista;
	novo->setInfo(info);
	temp->fitInFront(novo);
	if (!novo->getNext())
		_ult = novo;
	_numElem++;
}

void* Lista::pop() {

	if (vazia())
		{printf("Erro (2) em modulo \"Lista.cpp\": pop em lista vazia.\n\n");	exit(1);}

	void* tempInfo = _ult->getInfo();
	NoLista* tempUlt = _ult;

	_ult = _ult->getPrevious();
	if (_ult == NULL)
		_prim = NULL;

	delete tempUlt;

	_numElem--;

	return tempInfo;
}

void Lista::retira(int pos) {

	if (pos < 0 || pos >= _numElem) {
		printf("Aviso (2) em modulo \"Lista.cpp\": posicao de retirada invalida. Nenhum elemento foi retirado.\n");
		return;
	}

	NoLista* temp;

	if (pos == 0 || pos == _numElem-1) {
		if (pos == 0) {

			temp = _prim;
			_prim = _prim->getNext();
		}
		if (pos == _numElem-1) {

			temp = _ult;
			_ult = _ult->getPrevious();
		}
	} else
		temp = jumpToPos(pos);

	delete temp;

	_numElem--;
}

void* Lista::retiraPop(int pos) {

	if (pos < 0 || pos >= _numElem) {
		printf("Erro (4) em modulo \"Lista.cpp\": posicao de retirada pop invalida.\n\n");
		exit(1);
	}

	NoLista* temp;

	if (pos == 0 || pos == _numElem-1) {
		if (pos == 0) {

			temp = _prim;
			_prim = _prim->getNext();
		}
		if (pos == _numElem-1) {

			temp = _ult;
			_ult = _ult->getPrevious();
		}
	} else
		temp = jumpToPos(pos);

	void* tempInfo = temp->getInfo();
	delete temp;

	_numElem--;
	return tempInfo;
}

void*& Lista::operator[](int pos) {

	if (pos < 0 || pos >= _numElem) {
		if (vazia()) 
			{printf("Erro (3) em modulo \"Lista.cpp\": acesso em lista vazia.\n\n");	exit(1);}
		printf("Aviso (3) em modulo \"Lista.cpp\": posicao de acesso invalida. O primeiro elemento da lista foi acessado.\n");
		pos = 0;
	}

	NoLista* temp = jumpToPos(pos);
	return temp->getInfoRef();
}

bool criterioPadrao(void* elemBuscado, void* elemLista) {
	if (elemBuscado == elemLista)
		return 1;
	else
		return 0;
}

int Lista::busca(void* elemBuscado, bool (*criterio)(void* elemBuscado, void* elemLista)) {

	if (!criterio)
		criterio = criterioPadrao;

	int i = 0;
	NoLista* temp = _prim;

	while(temp) {
		if (criterio(elemBuscado, temp->getInfo()))
			return i;
		i++;
		temp = temp->getNext();
	}
	return -1;
}

int Lista::getSize() {

	return _numElem;
}