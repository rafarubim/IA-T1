#pragma once

#include "NoHeap.hpp"
#include "Lista.hpp"

class Heap {
private:

	NoHeap* _raiz;
	int _numElem;
	Lista _insercao;
	bool (*_comparacao)(void* a, void* b); // Retorno 1 -> a é prioridade, Retorno 0 -> b é prioridade (No caso a == b, retornar 0 otimiza o heap)

	void apagaSubArvore(NoHeap* subRaiz);
	void mostraNoHeap(NoHeap* no, void (imprime)(void*));
	NoHeap* newElem(NoHeap* pai, void* info);
	void updateNewElem();
	void updatePopElem();
	void corrigeAcima(NoHeap* no);
	void corrigeAbaixo(NoHeap* no);
	NoHeap* insereSemCorrigir(void* info); // Retorna nó inserido

public:

	Heap(bool (*comparacao)(void*, void*) = NULL);
	~Heap();

	void alteraPrioridade(bool (*comparacao)(void*, void*));

	int getNivelProxElem();
	int getNumElem();

	void insere(void* info);
	void* pop();

	void montaRapido(void** vec, int n);

	bool vazio();

	void debugImprime(void (imprime)(void*));
};