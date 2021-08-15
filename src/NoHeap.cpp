#include "NoHeap.hpp"

#include "stdio.h"
#include "stdlib.h"

NoHeap::NoHeap(NoHeap* pai) {

	_pai = pai;

	if (pai != NULL) {
		if (pai->_esq == NULL) {
			pai->_esq = this;
			_lado = ESQ;
		} else {
			pai->_dir = this;
			_lado = DIR;
		}
	} else
		_lado = NULO;

	_esq = _dir = NULL;
	_info = NULL;
}

NoHeap::~NoHeap() {

	if (_esq || _dir)
		{printf("Erro (3) em modulo \"NoHeap.cpp\": Um no no meio da arvore nao pode ser destruido!");	exit(1);}

	if (_pai)
		if (_lado == ESQ)
			_pai->_esq = NULL;
		else if (_lado == DIR)
			_pai->_dir = NULL;
		else
			{printf("Erro (4) desconhecido em modulo \"NoHeap.cpp\".\n\n");	exit(1);}
}

NoHeap* NoHeap::inicializaPai(NoHeap* pai) {

	if (_pai != NULL)
		{printf("Erro (2) em modulo \"NoHeap.cpp\": o pai ja existe, portanto nao pode ser inicializado!\n\n");	exit(1);}

	_pai = pai;
	
	if (pai != NULL) {
		if (pai->_esq == NULL) {
			pai->_esq = this;
			_lado = ESQ;
		} else {
			pai->_dir = this;
			_lado = DIR;
		}
	} else {
		_lado = NULO;
		printf("Aviso (2) em modulo \"NoHeap.cpp\": pai inicializado como NULL.\n\n");
	}
}

NoHeap* NoHeap::getParente(Direc parente) {

	switch(parente) {
	case ESQ:
		return _esq;
	case DIR:
		return _dir;
	case PAI:
		return _pai;
	default:
		printf("Aviso (1) em modulo \"NoHeap.cpp\": parente correto nao especificado. Pai retornado.\n\n");
		return _pai;
	}
}

NoHeap* NoHeap::getPai() {
	
	return _pai;
}

NoHeap* NoHeap::getFilhoEsq() {

	return _esq;
}

NoHeap* NoHeap::getFilhoDir() {

	return _dir;
}

Direc NoHeap::getLado() {

	return _lado;
}

NoHeap* NoHeap::trocaPai() {

	if (!_pai)
		{printf("Aviso (3) em modulo \"Noheap.cpp\": Troca com pai nao relizada!");	return NULL;}

	NoHeap* velhaPaiEsq = _pai->_esq;
	NoHeap* velhaPaiDir = _pai->_dir;

	_pai->_esq = _esq;
	_pai->_dir = _dir;

	if (_esq)
		_esq->_pai = _pai;
	if (_dir)
		_dir->_pai = _pai;

	NoHeap* velhoPaiPai = _pai->_pai;

	_pai->_pai = this;

	Direc velhoPaiLado = _pai->_lado;

	if (_lado == ESQ) {
		_esq = _pai;
		_pai->_lado = ESQ;
		_dir = velhaPaiDir;
		if (_dir)
			_dir->_pai = this;
	} else if (_lado == DIR) {
		_dir = _pai;
		_pai->_lado = DIR;
		_esq = velhaPaiEsq;
		if (_esq)
			_esq->_pai = this;
	} else
		{printf("Erro (1) grave desconhecido em \"NoHeap.cpp\"\n\n");	exit(1);}

	if (velhoPaiLado == ESQ) {
		velhoPaiPai->_esq = this;
		_lado = ESQ;
	} else if (velhoPaiLado == DIR) {
		velhoPaiPai->_dir = this;
		_lado = DIR;
	} else //Caso VelhoPaiPai == NULL
		_lado = NULO;

	NoHeap* velhoPai = _pai;
	_pai = velhoPaiPai;

	return velhoPai;
}

void NoHeap::setInfo(void* info) {

	_info = info;
}

void* NoHeap::getInfo() {

	return _info;
}