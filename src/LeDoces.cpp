#include "LeDoces.h"

#include "stdio.h"
#include "stdlib.h"
#include "Lista.hpp"

void leDoces(char* arqS, Lista &lstClareirasOrdem, Lista &lstDoces) {

	FILE* arq;
	int nDoces, nClar;
	arq = fopen(arqS, "rt");
	if (!arq) {printf("Erro em abertura de arquivo para ler doces.\n\n");	exit(1);}

	if (fscanf(arq, " %d", &nClar) != 1)
		{printf("Erro: Arquivo de doces em formato incorreto.\n\n");	exit(1);}
	for(int i = 0; i < nClar; i++) {
		float* novoCusto = new float;
		if (fscanf(arq, " %f", novoCusto) != 1)
			{printf("Erro: Arquivo de doces em formato desconhecido.\n\n");	exit(1);}
		lstClareirasOrdem.push(novoCusto);
	}

	while (fscanf(arq, " %d", &nDoces) == 1) {
		Lista* listaDoces = new Lista;
		lstDoces.push(listaDoces);
		for(int i = 0; i < nDoces; i++) {
			float* novoDoce = new float;
			if (fscanf(arq, " %f", novoDoce) != 1)
				{printf("Erro em formato de arquivo de doces.\n\n");	exit(1);}
			((Lista*)lstDoces[lstDoces.getSize()-1])->push(novoDoce);
		}
	}
}