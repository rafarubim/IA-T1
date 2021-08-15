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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define FAM 100
#define LN 10
#define COL 5
#define ZER 50

int temZero(unsigned char *s);
/*
	Checa se na funcao, existe alguma clareira com nenhum doce
*/
void escreve(FILE *arq, unsigned char *s, float soma, float *fome, int geracao);
/*
	Escreve em um arquivo, um log dos melhores resultados do Teste efetuado
*/
int checa_bits(char vetbit, char bit);
/*
	Checa se numa clareira, existe o doce procurado na clareira. Retorna 1 se tiver o doce, 0 se nao tiver
*/
void acende(unsigned char *cla, char * val);
/*
	Caso haja uma clareira sem doce, a funcao ira procurar nas outras clareiras, algum doce para passar essa clareira.
*/
int auxexibe(unsigned char s, float *doces);
/*
	Funcao que converte um vetor de bits para valores float respectivos aos doces
*/
void exibe_clareira(unsigned char *cla, float *fome);
/*
	Funcao que exibe todas as clareiras, e os respectivos doces em cada clareira
*/
float obtem_doces(int index);
/*
	Vetor com valor de cada doce
*/
void obtem_fome(float *fome);
/*
	Funcao responsavel por ler o arquivo fome.txt que tem os valores
*/
void checa_certo(unsigned char *cla);
/*
	Funcao que checa se a combinacao de clareiras gerada, esta válida
*/
void troca_gulosa(unsigned char *s, float *fome);
/*
	Funcao que realiza varias trocas, de forma 100% gulosa
*/
int mutacao(int n);
/*
	Funcao mais usada nos testes, para gerar diferentes tipos de mutacao
*/
float obtem_soma(unsigned char *cla, float *fome);
/*
	Funcao que calcula, e retorna, o valor total somado em cada clareira
*/
int troca_bits(unsigned char *cla, unsigned char bit1, int pos1, unsigned char bit2, int pos2);
/*
	Troca 2 doces de posicao. Checa se a troca é válida ou nao.
*/
int troca_char(unsigned char *cla, int pos1, int pos2);
/*
	Troca todos os doces de uma clareira, com o de outra. Checa se a troca é válida ou nao.
*/
int troca_leva(unsigned char *cla, unsigned char bit, int pos1, int pos2);
/*
	Pega o doce de uma clareira, e leva até outra. Checa se a troca é válida ou nao.
*/
void trocas(unsigned char *cla, int n, int max);
/*
	Funcao que junta todos os tipos de trocas, e a repete em uma determinada ordem.
*/
void copia_m(unsigned char cla[FAM][LN], unsigned char s[FAM][LN], int i);
/*
	Funcao usada para copiar dois grupos de combinacoes de clareiras.
*/
void copia_string(unsigned char *a, unsigned char *b, int n);
/*
Funcao usada para copiar duas combinacoes de clareiras.
*/
void troca_ordena(unsigned char s[FAM][LN], int pos1, int pos2);
/*
	Funcao auxiliar da funcao Ordena. Troca dois combinacoes de clareiras de lugar.
*/
void ordena(unsigned char s[FAM][LN], int n, float *fome);
/*
	Ordena a matriz Combinacao de Clareiras.
*/
void recombina(unsigned char *s1, unsigned char *s2, int param);
/*
	Funcao que funde 2 vetores de bits, gerando 2 novos vetores.
	Funcao CrossOver
*/
void escreve_best(FILE *best, float verybest, unsigned char *s);
int obtem_melhor_soma(unsigned char pais[FAM][LN], float *fome);
/*
	Obtem a melhor soma dentro de um grupo de combinacao de clareiras.
*/
void ordem_trocas(unsigned char *s, unsigned char bit1, unsigned char bit2, int pos1, int pos2, int ordem);
/*
	Funcao auxiliar da trocas, para indicar a ordem que devera ser usada as funcoes de trocas de doces.
*/
void primeira_troca(unsigned char *s);
/*
	Funcao responsavel por realizar as primeiras trocas, para gerar o primeiro grupo de combinacao de clareiras, a partir de mutacoes de uma clareira original.
*/