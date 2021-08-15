#include "Mapa.hpp"
#include "time.h"

void Mapa::calculaCaminhoFinal(Lista &caminho) {

	NoMapa* atual = _inicial;
	Lista temp;
	int menorPreco = -1;
	srand(time(NULL));

	while (atual != _final) {

		atual->abreVizinhos(temp);

		int qtd = 1;
		for (int i = 0; i < temp.getSize(); i++) {
			int preco = ((NoMapa*)temp[i])->getCusto() + ((NoMapa*)temp[i])->getDistFinal();
			if (menorPreco == -1 || preco <= menorPreco) {
				if (preco == menorPreco)
					qtd += 1;
				else
					qtd = 1;
				menorPreco = preco;
			}
		}
		while(!temp.vazia()) {
			NoMapa* no = (NoMapa*) temp.pop();
			if (no->getCusto() + no->getDistFinal() == menorPreco) {
				if (rand() % qtd == 0) {
					caminho.push(no);
					atual = no;
					break;
				} else
					qtd--;
			}
		}

		while(!temp.vazia())
			temp.pop();
	}
}