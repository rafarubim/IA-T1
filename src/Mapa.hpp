#pragma once

#include "NoMapa.hpp"
#include "Lista.hpp"

class Mapa {

private:

	int _width, _height;
	NoMapa *_inicial, *_pos, *_final;
	int _inicialX, _inicialY, _finalX, _finalY;
	NoMapa ***_casas;
	Lista _clareiras;

public:

	Mapa();
	~Mapa();

	void loadMapa(char* arquivo);
	void printDistancias(char* arquivo);

	int getWidth();
	int getHeight();
	int getCustoNo(int i, int j);
	int getInicialX();
	int getInicialY();
	int getFinalX();
	int getFinalY();

	Lista& getClareiras();

	void calculaDistancias();
	void calculaRota(Lista &abertos, Lista &caminho);
	void calculaCaminhoFinal(Lista &caminho);
};