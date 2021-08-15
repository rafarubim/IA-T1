#pragma once

#include "NoLista.hpp"

class Lista {
private:

	NoLista* _prim;
	NoLista* _ult;
	int _numElem;

	NoLista* jumpToPos(int pos);

public:

	Lista();
	~Lista();

	bool vazia();

	void push(void* info);
	void insere(int pos, void* info);

	void* pop();
	void retira(int pos);
	void* retiraPop(int pos);

	void*& operator[](int pos);

	int busca(void* elemBuscado, bool (*criterio)(void* elemBuscado, void* elemLista) = NULL); // Retorna o indice do elemento procurado ou -1 se inexistente. "criterio" é a função que retorna 1 caso seu parâmetro corresponda ao que se procura (O tipo de elemBuscado pode ser diferente do tipo padrão da lista)

	int getSize();
};