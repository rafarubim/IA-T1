#include "Heap.hpp"

#include "stdio.h"
#include "stdlib.h"

bool compPadrao(void* a, void* b) {

	if (*(int*) a > *(int*)b)
		return 1;
	else
		return 0;
}

Heap::Heap(bool (*comparacao)(void*, void*)) {

	_raiz = NULL;
	_numElem = 0;
	if (!comparacao)
		_comparacao = compPadrao;
	else
		_comparacao = comparacao;
}

Heap::~Heap() {

	for (int i = 0; i < _insercao.getSize(); i++)
		delete _insercao[i];
	apagaSubArvore(_raiz);
}

void Heap::apagaSubArvore(NoHeap* subRaiz) {

	if (!subRaiz)
		return;

	apagaSubArvore(subRaiz->getFilhoEsq());
	apagaSubArvore(subRaiz->getFilhoDir());
	delete subRaiz;
}

void Heap::mostraNoHeap(NoHeap* no, void (imprime)(void*)) {

	if (no == NULL)
		{printf("-");	return;}

	imprime(no->getInfo());
	printf("(");
	mostraNoHeap(no->getFilhoEsq(), imprime);
	printf(",");
	mostraNoHeap(no->getFilhoDir(), imprime);
	printf(")");
}

NoHeap* Heap::newElem(NoHeap* pai, void* info) {

	NoHeap* novo = new NoHeap(pai);
	novo->setInfo(info);
	updateNewElem();
	_numElem++;
	return novo;
}

void Heap::updateNewElem() {
	
	if (_insercao.vazia()) {
		_insercao.push(new Direc(ESQ));
	} else {
		int* temp = (int*) _insercao.pop();
		if (*temp == ESQ) {
			*temp = DIR;
			_insercao.push(temp);
		} else if (*temp == DIR) {
			updateNewElem();
			*temp = ESQ;
			_insercao.push(temp);
		} else {
			printf("Erro (1) desconhecido em modulo \"Heap.cpp\".\n\n");	exit(1);
		}
	}
}

void Heap::updatePopElem() {

	if (_insercao.vazia()) {
		printf("Erro (2) em modulo \"Heap.cpp\": retirada de heap vazio\n\n");	exit(1);
	} else {
		Direc* temp = (Direc*) _insercao.pop();
		if (*temp == DIR) {
			*temp = ESQ;
			_insercao.push(temp);
		} else if (*temp == ESQ) {
			if (!_insercao.vazia()) {
				updatePopElem();
				*temp = DIR;
				_insercao.push(temp);
			} else
				delete temp;
		} else {
			printf("Erro (3) desconhecido em modulo \"Heap.cpp\".\n\n");	exit(1);
		}
	}
}

void Heap::corrigeAcima(NoHeap* no) {

	while(no->getPai() && _comparacao(no->getInfo(), no->getPai()->getInfo()))
		if (no->trocaPai() == _raiz)
			_raiz = no;
}

void Heap::corrigeAbaixo(NoHeap* no) {

	while(NoHeap* esq = no->getFilhoEsq()) {
		Direc prioridade = ESQ;
		NoHeap* dir = no->getFilhoDir();
		if (dir && _comparacao(dir->getInfo(), esq->getInfo()))
			prioridade = DIR;
		NoHeap* noPrioritario = no->getParente(prioridade);
		if (_comparacao(noPrioritario->getInfo(), no->getInfo())) {
			noPrioritario->trocaPai();
			if (no == _raiz)
				_raiz = noPrioritario;
		} else
			break;
	}
}

NoHeap* Heap::insereSemCorrigir(void* info) {

	if (_raiz == NULL) {

		_raiz = newElem(NULL, info);
		return _raiz;

	} else {

		NoHeap* temp = _raiz;
		for (int i = 0; i < _insercao.getSize()-1; i++)
			temp = temp->getParente(*(Direc*)_insercao[i]);
		temp = newElem(temp, info);
		return temp;
	}
}

void Heap::alteraPrioridade(bool (*comparacao)(void*, void*)) {

	_comparacao = comparacao;

	Lista listaNos;
	listaNos.push(_raiz);
	int i = 0;
	while(1) {

		NoHeap* esq = ((NoHeap*)listaNos[i])->getFilhoEsq();
		if (!esq)
			break;
		listaNos.push(esq);
		NoHeap* dir = ((NoHeap*)listaNos[i])->getFilhoDir();
		if (!dir)
			break;
		listaNos.push(dir);
		i++;
	}

	while(!listaNos.vazia())
		corrigeAbaixo((NoHeap*)listaNos.pop());
}

int Heap::getNivelProxElem() {

	return _insercao.getSize();
}

int Heap::getNumElem() {

	return _numElem;
}

void Heap::insere(void* info) {

	NoHeap* novo = insereSemCorrigir(info);
	corrigeAcima(novo);
}

void* Heap::pop() {

	if (_numElem == 0)
		{printf("Erro (4) em modulo \"Heap.cpp\": pop em heap vazio.\n\n");	exit(1);}

	void* tempInfo = _raiz->getInfo();
	updatePopElem();

	NoHeap* temp = _raiz;
	for (int i = 0; i < _insercao.getSize(); i++)
			temp = temp->getParente(*(Direc*)_insercao[i]);

	if (temp != _raiz)
		_raiz->setInfo(temp->getInfo());
	else
		_raiz = NULL;
	delete temp;
	_numElem--;

	if (!vazio())
		corrigeAbaixo(_raiz);

	return tempInfo;
}

void Heap::montaRapido(void** vec, int n) {

	Lista novos;

	for (int i = 0; i < n; i++)
		novos.push(insereSemCorrigir(vec[i]));

	for (int i = 0; i < n; i++)
		corrigeAbaixo((NoHeap*) novos.pop());
}

bool Heap::vazio() {

	if (_numElem == 0)
		return 1;
	else
		return 0;
}

void Heap::debugImprime(void imprime(void*)) {

	mostraNoHeap(_raiz, imprime);
}