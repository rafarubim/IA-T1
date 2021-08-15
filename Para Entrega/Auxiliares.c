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
/*	Data de Entrega: 10/Oct/2016																							*/
/*																															*/
/*																															*/
/****************************************************************************************************************************/

#include "Auxiliares.h"

void obtem_valores_doces(char *val)
{
	val[0] = 0x01; val[1] = 0x02; val[2] = 0x04; val[3] = 0x08; val[4] = 0x10;
}
int mutacao(int n)
{
	//srand((unsigned)time(NULL));
	return rand() % n;
	//return (GetTickCount())%n;
}
void escreve(FILE *arq, unsigned char *s, float soma, float *fome, int geracao)
{
	int i, j, n;
	float doces[COL];
	fprintf(arq, "========= Geracao: %d =========\n", geracao);
	for (i = 0; i < LN; i++)
	{
		n = auxexibe(s[i], doces);
		if ((i + 1) < 10)fprintf(arq, "Clareira 0%d\tFome: %.0f\tDoces: ", i + 1, fome[i]);
		else fprintf(arq, "Clareira %d\tFome: %.0f\tDoces: ", i + 1, fome[i]);
		for (j = 0; j < n; j++)
		{
			fprintf(arq, "%.1f\t", doces[j]);
		}
		fprintf(arq, "\n");
	}
	fprintf(arq, "Soma: %.04f\n", obtem_soma(s, fome));
}
void obtem_fome(float *fome)
{
	FILE *arq;
	int i, aux;
	arq = fopen("fome.txt", "r");
	if (arq == NULL)
	{
		printf("Erro na abertura do arquivo - fome.txt\n");
		exit(1);
	}
	i = 0;
	while (i < LN && fscanf(arq, "%d", &aux) == 1)
	{
		fome[i] = (float)aux;
		i++;
	}
}
void acende(unsigned char *cla, char *val)
{
	int i, a, pos;
	for (i = 0; i < LN; i++)
	{
		if (cla[i] == 0x00)
		{
			a = mutacao(COL);
			pos = buscaRNG(cla, val[a]);
			cla[pos] = cla[pos] - val[a];
			cla[i] = val[a];
		}
	}

}
void checa_certo(unsigned char *cla)
{
	int i, j, n = 0;
	float aux;
	float doces[COL];
	int checador[COL];
	unsigned char aux2;
	for (j = 0; j < COL; j++) checador[j] = 0;
	for (i = 0; i < LN; i++)
	{
		if (cla[i] == 0x00) n++;
		if (cla[i] > 0x1F)
		{
			printf("Error: Matriz Invalida - Mais de %d elementos em 1 Clareira\n", COL);
			exit(1);
		}
		for (j = 0; j < auxexibe(cla[i], doces); j++)
		{
			checador[(int)((doces[j] * 10) - 11)]++;
		}
	}
	if (n == LN - 1)
	{
		printf("Error: Matriz Invalida - Matriz Nula\n");
		exit(1);
	}
	for (j = 0; j < COL; j++)
	{
		if (checador[j] > 5)
		{
			printf("Error: Matriz Invalida - Mais de %d de um elemento em todas as clareiras\n", COL);
			exit(1);
		}
	}
}
int checa_bits(char vetbit, char bit)
{
	if ((vetbit & bit) != 0x00)
	{
		return 1;
	}
	return 0;
}
int buscaRNG(unsigned char *cla, char elemen)
{
	int i, j;
	int v[LN];
	for (i = 0; i < LN; i++)
	{
		v[i] = -1;
	}
	for (i = 0; i <LN; i++)
	{
		if (checa_bits(cla[i], elemen) == 1)
		{
			v[i] = i;
		}
	}
	while (1)
	{
		i = mutacao(LN);
		if (v[i] != -1)
		{
			return v[i];
		}
	}
}
float obtem_soma(unsigned char *cla, float *fome)
{
	float soma;
	float somadoces;
	int i, j, n;
	float doces[COL];
	soma = 0;
	for (i = 0; i < LN; i++)
	{
		somadoces = 0;
		n = auxexibe(cla[i], doces);
		for (j = 0; j < n; j++)
		{
			somadoces += doces[j];
		}
		if (somadoces != 0)
		{
			soma = soma + (fome[i] / somadoces);
		}
		else
		{
			soma = soma + fome[i];
		}
	}
	if (soma == 0)
	{
		printf("Erro na Soma\n");
		exit(1);
	}
	return soma;
}
void exibe_clareira(unsigned char *cla, float *fome)
{
	int i, j, n;
	float doces[COL];
	unsigned char aux;
	int SOMA = 1;
	for (i = 0; i <LN; i++)
	{
		n = auxexibe(cla[i], doces);
		if (SOMA == 0)
		{
			if ((i + 1) < 10)printf("Clareira 0%d\tFome: %.0f\tDoces: ", i + 1, fome[i]);
			else printf("Clareira %d\tFome: %.0f\tDoces: ", i + 1, fome[i]);
		}
		for (j = 0; j < n; j++)
		{
			if (SOMA == 0)printf("%.1f\t", doces[j]);

		}
		if (SOMA == 0)printf("\n");
	}
	printf("Soma: %.04f\n", obtem_soma(cla, fome));
}
int auxexibe(unsigned char s, float *doces)
{ //i=0 -> 1 // i=1 -> 2 // i=2 -> 4 // i=3 -> 8 // i=4 ->16
	int i, j;
	unsigned char a = 1;
	unsigned char aux = s;
	for (i = 0, j = 0; i < COL; i++)
	{
		if ((a & aux) != 0)
		{
			doces[j] = obtem_doces(i);
			j++;
		}
		a = (a << 1);
		if (j == COL) break;
	}
	return j;
}
float obtem_doces(int index)
{
	float a[6];
	a[5] = 0.0f;
	a[0] = 1.1f;
	a[1] = 1.2f;
	a[2] = 1.3f;
	a[3] = 1.4f;
	a[4] = 1.5f;
	return a[index];
}
int troca_bits(unsigned char *cla, unsigned char bit1, int pos1, unsigned char bit2, int pos2)
{
	if (cla == NULL)
	{
		printf("Error na Binario\n");
		exit(1);
	}
	if (checa_bits(cla[pos1], bit1) == 1 && checa_bits(cla[pos2], bit2) == 1)
	{
		if (checa_bits(cla[pos1], bit2) == 0 && checa_bits(cla[pos2], bit1) == 0)
		{
			cla[pos1] = cla[pos1] - bit1 + bit2;
			cla[pos2] = cla[pos2] - bit2 + bit1;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}
int troca_char(unsigned char *cla, int pos1, int pos2)
{
	if (cla == NULL)
	{
		printf("Error na Binario\n");
		exit(1);
	}
	//printf("x%02x - x%02x\n", cla[pos1], cla[pos2]);
	if (cla[pos1] == cla[pos2])
	{
		return 0;
	}
	unsigned char aux;
	aux = cla[pos1];
	cla[pos1] = cla[pos2];
	cla[pos2] = aux;
	//printf("x%02x - x%02x\n\n\n", cla[pos1], cla[pos2]);
	return 1;
}
int troca_leva(unsigned char *cla, unsigned char bit, int pos1, int pos2)
{
	if (cla == NULL)
	{
		printf("Error na Binario\n");
		exit(1);
	}
	if (cla[pos1] == cla[pos2]) return 0;
	if (checa_bits(cla[pos1], bit) == 1 && checa_bits(cla[pos2], bit) == 0)
	{
		cla[pos1] = cla[pos1] - bit;
		cla[pos2] = cla[pos2] + bit;
		return 1;
	}
	else
	{
		if (checa_bits(cla[pos1], bit) == 0 && checa_bits(cla[pos2], bit) == 1)
		{
			cla[pos2] = cla[pos2] - bit;
			cla[pos1] = cla[pos1] + bit;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
void trocas(unsigned char *cla, int n, int max)
{
	int a, b, pos1, pos2;
	int i, j, ordem, certo;
	int NElementosApos[LN];
	unsigned char s[LN];
	unsigned char val[COL];
	float doces[COL];
	obtem_valores_doces(val);
	if (n > max / 2)
	{
		for (i = 0; i < n * 2; i++)
		{
			a = mutacao(COL);
			pos1 = buscaRNG(cla, val[a]);
			b = mutacao(COL);
			pos2 = buscaRNG(cla, val[b]);
			ordem = mutacao(3);
			ordem_trocas(cla, val[a], val[b], pos1, pos2, ordem);
		}
	}
	else
	{
		for (i = (n * 2); i > 0; i--)
		{
			a = mutacao(COL);
			pos1 = buscaRNG(cla, val[a]);
			b = mutacao(COL);
			pos2 = buscaRNG(cla, val[b]);
			ordem = mutacao(3);
			ordem_trocas(cla, val[a], val[b], pos1, pos2, ordem);
		}
	}
	for (i = 0, certo = 0; i<LN; i++)
	{
		NElementosApos[i] = auxexibe(cla[i], doces);
		if (NElementosApos[i] > 1 && NElementosApos[i] < 4)
		{
			certo++;
		}
	}
	if (certo == 9)
	{
		copia_string(s,cla, LN);
	}
}
void ordena(unsigned char s[FAM][LN], int n, float *fome)
{
	int i, j;
	int val1, val2;
	for (i = n - 1; i > -1; i--)
	{
		for (j = 0; j < i; j++)
		{
			val1 = obtem_soma(s[j], fome);
			val2 = obtem_soma(s[j + 1], fome);
			if (val1 > val2)
			{
				troca_ordena(s, j, j + 1);
			}

		}
	}
}
void copia_m(unsigned char cla[FAM][LN], unsigned char s[FAM][LN], int i)
{
	int a;
	for (a = 0; a < i; a++)
	{
		copia_string(cla[a], s[a], LN);
	}
}
void copia_string(unsigned char *a, unsigned char *b, int n)
{
	int i;
	if (a == NULL || b == NULL)
	{
		printf("Error na Copia\nParametros Nulos\n");
		exit(1);
	}
	for (i = 0; i < n; i++)
	{
		a[i] = b[i];
	}
}
void troca_gulosa(unsigned char *s, float *fome)
{
	int repet;
	int a, b, pos1, pos2;
	unsigned char val[COL];
	float somaANT, somaPROX;
	unsigned char aux[LN];
	obtem_valores_doces(val);
	repet = 50;
	somaANT = obtem_soma(s, fome);
	copia_string(aux, s, LN);
	while (repet > 0)
	{
		a = GetTickCount()%COL;
		pos1 = buscaRNG(aux, val[a]);
		b = mutacao(COL);
		pos2 = buscaRNG(aux, val[b]);
		troca_bits(aux, val[a], pos1, val[b], pos2);
		somaPROX = obtem_soma(aux, fome);
		if (somaPROX < somaANT)
		{
			copia_string(s, aux, LN);
			return;
		}
		troca_char(aux, pos1, pos2);
		somaPROX = obtem_soma(aux, fome);
		if (somaPROX < somaANT)
		{
			copia_string(s, aux, LN);
			return;
		}
		troca_leva(aux, val[a], pos1, pos2);
		somaPROX = obtem_soma(aux, fome);
		if (somaPROX < somaANT)
		{
			copia_string(s, aux, LN);
			return;
		}
		repet--;
	}

}
void recombina(unsigned char *s1, unsigned char *s2, int param)
{
	int i, n;
	n = param;
	unsigned char recomb[COL];
	recomb[1] = 0x01;
	recomb[2] = 0x03;
	recomb[3] = 0x07;
	recomb[4] = (recomb[3] << 1) + 1;
	if (n == 0) n = mutacao(COL - 1) + 1;
	unsigned char aux, aux1, aux2, aux3;
	for (i = 0; i < LN; i++)
	{
		aux = s1[i];
		aux = (aux >> n) << n;
		aux1 = s1[i] & recomb[n];

		aux2 = s2[i];
		aux2 = (aux2 >> n) << n;
		aux3 = s2[i] & recomb[n];

		s1[i] = (aux | aux3) & 0x1F;
		s2[i] = (aux2 | aux1) & 0x1F;
	}
}
void troca_ordena(unsigned char s[FAM][LN], int pos1, int pos2)
{
	unsigned char aux[LN];
	int i;
	float docesaux[COL];
	copia_string(aux, s[pos1], LN);
	copia_string(s[pos1], s[pos2], LN);
	copia_string(s[pos2], aux, LN);
}
int temZero(unsigned char *s)
{
	int i;
	for (i = 0; i < LN; i++)
	{
		if (s[i] == 0)
		{
			return 0;
		}
	}
	return 1;
}
int compara_string(unsigned char *s1, unsigned char *s2)
{
	int i;
	for (i = 0; i < LN; i++)
	{
		if (s1[i] != s2[i])
		{
			return 0;
		}
	}
	return 1;
}
void ajeita_zero(unsigned char s[FAM][LN], unsigned char *s1, int *repeticoes)
{
	int i;
	for (i = 0; i < FAM; i++)
	{
		if (s[i][0] == s1[0])
		{
			repeticoes[i]++;
			if (repeticoes[i] >= ZER)
			{
				troca_char(s1, 0, mutacao(COL));
				repeticoes[i] = 0;
				return;
			}
		}
	}
}
void escreve_best(FILE *best, float verybest, unsigned char *s)
{
	int i, j;
	checa_certo(s);
	fprintf(best, "%.04f\t", verybest);
	for (i = 0; i < LN - 1; i++)
	{
		fprintf(best, "%02x\t", s[i]);
	}
	fprintf(best, "%02x\n", s[LN - 1]);
}
int obtem_melhor_soma(unsigned char pais[FAM][LN], float *fome)
{
	int i, indexador;
	float melhorsoma = 99999;
	float aux;
	for (i = 0; i < FAM; i++)
	{
		aux = obtem_soma(pais[i], fome);
		if (aux < melhorsoma)
		{
			melhorsoma = aux;
			indexador = i;
		}
	}
	return indexador;
}
void ordem_trocas(unsigned char *s, unsigned char bit1, unsigned char bit2, int pos1, int pos2, int ordem)
{
	if (ordem == 0)
	{
		troca_bits(s, bit1, pos1, bit2, pos2);
	}
	else if (ordem == 1)
	{
		troca_char(s, pos1, pos2);
	}
	else if (ordem == 2)
	{
		troca_leva(s, bit1, pos1, pos2);
	}
}
void primeira_troca(unsigned char *s)
{
	int a, b, pos1, pos2;
	int i, j, ordem;
	unsigned char val[COL];
	obtem_valores_doces(val);
	for (i = 0; i < 200; i++)
	{
		a = mutacao(COL);
		pos1 = buscaRNG(s, val[a]);
		b = mutacao(COL);
		pos2 = buscaRNG(s, val[b]);
		ordem = mutacao(3);
		ordem_trocas(s, val[a], val[b], pos1, pos2, ordem);
	}
}