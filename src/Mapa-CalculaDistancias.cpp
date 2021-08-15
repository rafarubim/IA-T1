#include "Mapa.hpp"

#include "Lista.hpp"
#include "Heap.hpp"
#include "stdio.h"
#include "stdlib.h"

typedef struct noHeuristico {
	int custoTotal;
	NoMapa* no;
} NoHeuristico;

bool prioridadesHeap(void* a, void* b) {

	NoHeuristico* cA = (NoHeuristico*) a;
	NoHeuristico* cB = (NoHeuristico*) b;

	if (cA->custoTotal < cB->custoTotal)
		return 1;
	else
		return 0;
}

void Mapa::calculaDistancias() {

	_final->setDistFinal(0);
	Lista nosACalcular;

	_final->abreVizinhos(nosACalcular);

	while(!nosACalcular.vazia()) {

		Lista temp;
		NoMapa* noAtual = (NoMapa*)nosACalcular.pop();
		noAtual->abreVizinhos(temp);

		Lista nosVisitados;
		Heap caminhosAAbrir(prioridadesHeap);

		int menorDist = -1;

		while(!temp.vazia()) {
			NoHeuristico* caminho = new NoHeuristico;
			caminho->no = (NoMapa*) temp.pop();
			caminho->custoTotal = caminho->no->getCusto();

			int heuristica = caminho->no->getDistFinal();
			if (heuristica != -1) {
				int distanciaDoCaminho = caminho->custoTotal + heuristica;
				if (menorDist == -1)
					menorDist = distanciaDoCaminho;
				else
					if (menorDist > distanciaDoCaminho)
						menorDist = distanciaDoCaminho;
			} else {
				nosVisitados.push(caminho->no);
				if (menorDist == -1 || (menorDist != -1 && caminho->custoTotal < menorDist))
					caminhosAAbrir.insere(caminho);
			}
		}

		while(!caminhosAAbrir.vazio()) {

			NoHeuristico* caminhoAtual = (NoHeuristico*)caminhosAAbrir.pop();

			if (!temp.vazia())
				{printf("Erro (1) desconhecido em modulo \"Mapa-CalculaDistancias.cpp\".\n\n");	exit(1);}

			caminhoAtual->no->abreVizinhos(temp);

			while(!temp.vazia()) {
				noHeuristico* novoCaminho = new NoHeuristico;
				novoCaminho->no = (NoMapa*) temp.pop();

				int indexBusca = nosVisitados.busca(novoCaminho->no);
				if (indexBusca == -1) {
					novoCaminho->custoTotal = caminhoAtual->custoTotal + novoCaminho->no->getCusto();
					int heuristica = novoCaminho->no->getDistFinal();
					if (heuristica != -1) {
						int distanciaDoNovoCaminho = novoCaminho->custoTotal + heuristica;
						if (menorDist == -1) 
							menorDist = distanciaDoNovoCaminho;
						else
							if (menorDist > distanciaDoNovoCaminho)
								menorDist = distanciaDoNovoCaminho;
					} else {
						nosVisitados.push(novoCaminho->no);
						if (menorDist == -1 || (menorDist != -1 && novoCaminho->custoTotal < menorDist))
							caminhosAAbrir.insere(novoCaminho);
					}
				} else
					delete novoCaminho;
			}

			delete caminhoAtual;
		}

		if (menorDist == -1) {

			printf("Aviso (1) em modulo \"Mapa-CalculaDistancias.cpp\": para o no atual, foi impossivel calcular distancia minima ate o final (ha um caminho viavel?)\n");
			noAtual->setDistFinal(-1);
		} else
			noAtual->setDistFinal(menorDist);

		if (!temp.vazia())
			{printf("Erro (2) desconhecido em modulo \"Mapa-CalculaDistancias.cpp\".\n\n");	exit(1);}

		noAtual->abreVizinhos(temp);

		while(!temp.vazia()) {

			NoMapa* noACalcular = (NoMapa*) temp.pop();
			if (noACalcular->getDistFinal() == -1)
				nosACalcular.push(noACalcular);
		}
	}
}