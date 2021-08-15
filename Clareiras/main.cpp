#include <iostream>
using namespace std;
#include "Lista.hpp"
#include "time.h"

typedef struct sPerm {
	Lista listaP;
	float* num;
} Perm;

#define MAX 100000000

bool criterio(void* elemBuscado, void* elemLista) {
	float a = *(float*) elemBuscado;
	float b = *(float*) elemLista;
	if (a == b)
		return 1;
	else
		return 0;
}

void permutacoes(Lista& nova, Lista& temp) {
	if (temp.getSize() < 1)
		{printf("Sua recursiva deu errado seu bobao\n\n");	exit(1);}
	if (temp.getSize() == 1) {
		Perm* ultimo = new Perm;
		ultimo->num = (float*)temp[0];
	}
	for (int i = 0; i < temp.getSize()-1; i++) {
		Perm* perm = new Perm;
		nova.push(perm);
		void* tempPointer = temp.retiraPop(i);
		perm->num = (float*) tempPointer;
		permutacoes(perm->listaP, temp);
		temp.insere(i, tempPointer);
	}
}

bool listaMonta(Lista& destino, Lista& permutacoes) {

	Perm* first = (Perm*) destino[0];
	destino.push(first->num);
	if (first->listaP.vazia()) {
		delete first;
		return 1; // flag
	} else {
		int results = listaMonta(destino, first->listaP);
		if (results == 1) {
			first->listaP.retira(0);
			if (first->listaP.vazia()) {
				delete first;
				return 1;
			} else
				return 0;
		} else
			return 0;
	}
}

int main() {

	Lista cesta;
	Lista temp;
	Lista resultadosFinais[10];
	/*Lista permutacao;*/

	srand(time(NULL));

	float doces[] = {1.5, 1.4, 1.3, 1.2, 1.1};

	float c[] = {150, 140, 130, 120, 110, 100, 95, 90, 85, 80};

	Lista resultados[10];

	float menorSoma = 1100;

	//for (int i = 0; i < 24; i ++)
	//		temp.push(doces+i/5);
/*
	permutacoes(permutacao, temp);*/

	for (int i = 0; i < MAX; i++)
	/*while (!permutacao.vazia())*/ {

		for (int i = 0; i < 24; i ++)
			temp.push(doces+i/5);

		while(!temp.vazia()) {

			int ind = rand()%temp.getSize();
			cesta.push(temp.retiraPop(ind));
		}
	/*	listaMonta(cesta, permutacao);*/

		while (!cesta.vazia()) {

			float* doce = (float*) cesta.pop();

			float maiorDesconto = 0;
			int indexClareira = -1;
			for (int i = 0; i < 10; i++) {
				float sum = 0;
				for(int j = 0; j < resultados[i].getSize(); j++)
					sum += *(float*) resultados[i][j];
				float valorAntes;
				if (sum != 0)
					valorAntes = c[i]/sum;
				else
					valorAntes = c[i];
				float valorDepois = c[i]/(sum+*doce);
				if (valorAntes-valorDepois > maiorDesconto && resultados[i].busca(doce, criterio) == -1) {
					maiorDesconto = valorAntes-valorDepois;
					indexClareira = i;
				}
			}
			if (indexClareira == -1)
					{cout << "Erro desconhecido (1)\n\n";	exit(1);}
			resultados[indexClareira].push(doce);
		}

		float sumClareiras = 0;
		for(int i = 0; i < 10; i++) {
			////printf("Doces na Clareira %d: {", i+1);
			float sum = 0;
			for(int j = 0; j < resultados[i].getSize(); j++) {
				float doce = *(float*)resultados[i][j];
				sum += doce;
				//printf("%.2f, ", doce);
			}
			if (sum != 0)
				sumClareiras += c[i]/sum;
			else
				sumClareiras += c[i];
			//printf("}\n");
		}
		////printf("\nCusto total encontrado: %.2f\n\n", sumClareiras);
		if (sumClareiras < menorSoma) {
			menorSoma = sumClareiras;
			for (int i = 0; i < 10; i++)
				while(!resultadosFinais[i].vazia())
					resultadosFinais[i].pop();
			for(int i = 0; i < 10; i++)
				for(int j = 0; j < resultados[i].getSize(); j++)
					resultadosFinais[i].push(resultados[i][j]);
		}

		for(int i = 0; i < 10; i++)
			while(!resultados[i].vazia())
				resultados[i].pop();
	}

	printf("Menor soma: %.5f\n\n", menorSoma);

	float sumClareiras = 0;
	for(int i = 0; i < 10; i++) {
		printf("Doces na Clareira %d: {", i+1);
		float sum = 0;
		for(int j = 0; j < resultadosFinais[i].getSize(); j++) {
			float doce = *(float*)resultadosFinais[i][j];
			sum += doce;
			printf("%.2f, ", doce);
		}
		if (sum != 0)
			sumClareiras += c[i]/sum;
		else
			sumClareiras += c[i];
		printf("}\n");
	}

	return 0;
}