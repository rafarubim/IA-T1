#include "Mapa.hpp"

#include "stdio.h"
#include "stdlib.h"
#include "Lista.hpp"
#include "Pos.h"
#include "Heap.hpp"

typedef struct noHeuristico {
	NoMapa* no;
	int custoTotal;
} NoHeuristico;

bool compara(void* a, void* b) {
	NoHeuristico* cA = (NoHeuristico*) a;
	NoHeuristico* cB = (NoHeuristico*) b;

	if (cA->custoTotal+cA->no->getDistFinal() < cB->custoTotal+cB->no->getDistFinal())
		return 1;
	else
		return 0;
}

void Mapa::calculaRota(Lista &abertos, Lista &caminho) {

	Heap aAbrir(compara); // de nós heurísticos
	Lista temp; // para abrir nós
	NoMapa* noAtual = _inicial;

	noAtual->abreVizinhos(temp);

	while(!temp.vazia()) {
		NoHeuristico* caminhoPossivel = new NoHeuristico;
		caminhoPossivel->no = (NoMapa*) temp.pop();
		caminhoPossivel->custoTotal = caminhoPossivel->no->getCusto();
		abertos.push(caminhoPossivel->no);
		aAbrir.insere(caminhoPossivel);
	}

	while(!aAbrir.vazio()) {
		NoHeuristico* caminhoAtual = (NoHeuristico*) aAbrir.pop();
		
		if (caminho.busca(caminhoAtual->no) != -1) {
			delete caminhoAtual;
			continue;
		}

		caminho.push(caminhoAtual->no);
		if (caminhoAtual->no == _final) {
			delete caminhoAtual;
			break;
		}
		caminhoAtual->no->abreVizinhos(temp);
		while(!temp.vazia()) {
			NoHeuristico* caminhoPossivel = new NoHeuristico;
			caminhoPossivel->no = (NoMapa*) temp.pop();
			caminhoPossivel->custoTotal = caminhoPossivel->no->getCusto() + caminhoAtual->custoTotal;
			abertos.push(caminhoPossivel->no);
			aAbrir.insere(caminhoPossivel);
		}
		delete caminhoAtual;
	}
	while(!aAbrir.vazio())
		delete (NoHeuristico*) aAbrir.pop();
	for (int i = 0; i < caminho.getSize(); i++) {
		int indexAbertos = abertos.busca(caminho[i]);
		do {
			abertos.retira(indexAbertos);
			indexAbertos = abertos.busca(caminho[i]);
		} while(indexAbertos != -1);
	}
}