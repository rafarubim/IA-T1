/****************************************************************************************************************************/
/*																															*/
/*												Trabalho de Inteligencia Artificial											*/
/*															INF1900															*/
/*																															*/
/*	Trabalho 1: Calculo de Caminho de Menor Custo e Calculo de Melhor Combinacao de Clareiras								*/
/*	Professor: Augusto Baffa																								*/
/*	Grupo:	Alexandre Dias	- 1413183																						*/
/*			Rafael Cabral	- 1511068																						*/
/*																															*/
/*	Data de Entrega: 04/Oct/2016																							*/
/*																															*/
/****************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "Auxiliares.h"

#define TESTE 500
//Numero de geracoes que serao testadas
#define FAM 100
//Numero de Combinacoes de Clareiras em uma geracao
#define LN 10
//Numero de Clareiras
#define COL 5
//Numero de Tipos de Doces

void geradora(unsigned char matriz[FAM][LN]);
/*
	funcao que gera uma nova geracao
*/
void gera_filhos(unsigned char f[FAM][LN], float *fome, int n);
/*
	funcao que gera a primeira geracao
*/
void gera_outras_clareiras(unsigned char *s, int i);
/*
	funcao com clareiras predefinidas para usar como base.
*/
int teste_gula(unsigned char guloso[FAM][LN], float *fome, float *best, FILE *arq, unsigned char *bestclareira);
/*
	funcao que gera um grupo de combinacao de clareiras, submetidos a diversas trocas gulosas.
*/
void milagre(FILE *arq, FILE *best);
/*
	funcao milagrosa
*/
void saida(FILE *arq, unsigned char *s, float *fome);
/*
	funcao de saida do arquivo, com a combinacao ideal de doces para passar para a outra parte do programa.
*/

int main(void)
{
	unsigned char clareira_original[LN];
	float fome[LN];
	FILE *arq, *best;
	arq = fopen("map.txt", "w");
	if (arq == NULL)
	{
		printf("Erro na abertura do arquivo - map.Txt\n");
		exit(1);
	}
	best = fopen("melhores.txt", "w");
	if (best == NULL)
	{
		printf("Erro na abertura do arquivo - melhores.txt\n");
		exit(1);
	}
	obtem_fome(fome);
	milagre(arq, best);
	fclose(arq);
	fclose(best);
	return 0;
}
void gera_filhos(unsigned char f[FAM][LN], float *fome, int n)
{
	unsigned char filho1[LN], filho2[LN];
	int individuos = 0;
	int CoefMutacao;
	int mutation;
	unsigned char v[COL];
	obtem_valores_doces(v);
	mutation = ((n - (TESTE/2))*(n - (TESTE / 2)));
	while (individuos < FAM/2)
	{
		copia_string(filho1, f[individuos], LN);
		copia_string(filho2, f[individuos + 1], LN);
		recombina(filho1, filho2, mutacao(COL));
		CoefMutacao = mutacao(100);
		if (CoefMutacao <= mutation%100);
		{
			troca_gulosa(filho1, fome);
			trocas(filho2, individuos, FAM);
		}
		if (temZero(filho1) == 0) acende(filho1, v);
		if (temZero(filho2) == 0) acende(filho2, v);
		copia_string(f[(FAM - 1) - individuos], filho1, LN);
		individuos++;
		copia_string(f[(FAM - 1) - individuos], filho2, LN);
		individuos++;
		//printf("%d\n", individuos);
	}
}
void gera_outras_clareiras(unsigned char *cla, int i)
{
	if (i == 0) //382.3406
	{
		cla[0] = 0x10 + 0x08 + 0x04;
		cla[1] = 0x10 + 0x08 + 0x04;
		cla[2] = 0x10 + 0x08 + 0x04;
		cla[3] = 0x10 + 0x01 + 0x02;
		cla[4] = 0x10 + 0x08;
		cla[5] = 0x04 + 0x02;
		cla[6] = 0x04 + 0x02;
		cla[7] = 0x02 + 0x01;
		cla[8] = 0x02 + 0x01 + 0x08;
		cla[9] = 0x01;
	}
	if (i == 1) //347.5469
	{
		cla[0] = 0x0b;
		cla[1] = 0x0b;
		cla[2] = 0x07;
		cla[3] = 0x07;
		cla[4] = 0x18;
		cla[5] = 0x14;
		cla[6] = 0x18;
		cla[7] = 0x0c;
		cla[8] = 0x14;
		cla[9] = 0x12;
	}
}
void geradora(unsigned char matriz[FAM][LN])
{
	int i, j;
	unsigned char s[2][LN];
	gera_outras_clareiras(s[0], 0);
	gera_outras_clareiras(s[1], 1);
	for (i = 0; i < FAM; i++)
	{
		copia_string(matriz[i], s[i%2], LN);
		primeira_troca(matriz[i]);
		checa_certo(matriz[i]);
	}
}
int teste_gula(unsigned char matriz[FAM][LN], float *fome, float *best, FILE *arq, unsigned char *bestclareira)
{
	int i, j, index = -1;
	float melhor = *best;
	float temp;
	int trocas = 0;
	unsigned char aux[FAM][LN];
	copia_m(aux, matriz, FAM);
	for (i = 0; i < FAM; i++)
	{
		for (j = 0; j < 15; j++)
		{
			troca_gulosa(aux[i], fome);
			temp = obtem_soma(aux[i], fome);
			if (temp < melhor)
			{
				melhor = temp;
				index = i;
				trocas++;
			}
		}
	}
	if (index >= 0)
	{
		copia_string(bestclareira, aux[index], LN);
		escreve(arq, aux[index], melhor, fome, -1);
		*best = melhor;
	}
	return trocas;
}
void milagre(FILE *arq, FILE *best)
{
	unsigned char pais[FAM][LN];
	unsigned char filhos[FAM][LN];
	unsigned char bestclareira[LN];
	float fome[LN];
	obtem_fome(fome);
	float soma = 9999.99f;
	int inicio, final;
	int tempo;
	int i, geracao, sorte;
	int melhor;
	geradora(pais);
	ordena(pais, FAM, fome);
	copia_m(filhos, pais, FAM);
	for (geracao = 0; geracao < TESTE; geracao++)
	{
		final = GetTickCount();
		if (geracao == 1)
		{
			time_t rawtime;
			struct tm * timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			tempo = (TESTE*(final - inicio)) / 1000;
			printf("INICIO DO ARQUIVO\nTrabalho de Inteligencia Artificial\nGrupo: Alexandre(1413183) e Rafael(1511068)\nTempo de inicio da Compilacao: %s\n", asctime(timeinfo));
			printf("===================================================\n\n");
			printf("Numero de Geracoes que serao criadas no Teste: %d\nTempo aproximado para completar o teste: %d minuto(s) e %d segundos\n\nCarregando, Por Favor Aguarde.\n\n", TESTE, tempo / 60, tempo % 60);
			fprintf(arq, "INICIO DO ARQUIVO\nTrabalho de Inteligencia Artificial\nGrupo: Alexandre(1413183) e Rafael(1511068)\n");
			fprintf(arq, "Tempo da Compilacao: %s\n============================================\n", asctime(timeinfo));
		}
		inicio = GetTickCount();
		gera_filhos(filhos, fome, geracao);
		ordena(filhos, FAM, fome);
		melhor = obtem_melhor_soma(filhos, fome);
		if (obtem_soma(filhos[melhor], fome) < soma)
		{
			soma = obtem_soma(filhos[melhor], fome);
			copia_string(bestclareira, filhos[melhor], LN);
			if (geracao != 0)escreve(arq, filhos[melhor], soma, fome, geracao);
		}
		for (i = 0; i < 40; i++)
		{
			sorte = mutacao(100);
			if (sorte <= 20)
			{
				copia_string(pais[30 + i], filhos[i], LN);
			}
		}
		if (teste_gula(pais, fome, &soma, arq, bestclareira) == 0)
		{
			gera_filhos(pais, fome, geracao);
		}
		ordena(pais, FAM, fome);
		melhor = obtem_melhor_soma(pais, fome);
		if (obtem_soma(pais[melhor], fome) < soma)
		{
			soma = obtem_soma(pais[melhor], fome);
			copia_string(bestclareira, filhos[melhor], LN);
			escreve(arq, pais[melhor], soma, fome, geracao);
		}
	}
	printf("Melhor de Todas as Somas Encontradas nesse Teste: %.04f\n", soma);
	saida(best, bestclareira,fome);
}
void saida(FILE *arq, unsigned char *s, float *fome)
{
	float doces[COL];
	float soma = 0;
	int i,j, n;
	fprintf(arq, "%d\n", LN);
	for (i = 0; i < LN; i++)
	{
		fprintf(arq, "%d ", (int)fome[i]);
	}
	fprintf(arq, "\n");
	for (i = 0; i < LN; i++)
	{
		n = auxexibe(s[i], doces);
		fprintf(arq, "%d ", n);
		for (j = 0; j < n; j++)
		{
			fprintf(arq, "%.4f", doces[j]);
		}
		fprintf(arq, "\n");
	}
}