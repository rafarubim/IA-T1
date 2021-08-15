#pragma once

#include "Lista.hpp"

class NoMapa {
private:

	int _numVizinhos;
	NoMapa** _vizinhos;
	int _custo;
	int _distFinal;
	void* _info;

	void addVizinho(NoMapa* vizinho);
	void removeVizinho(NoMapa* vizinho);

public:

	NoMapa(int custo = 0);
	~NoMapa();

	void link(NoMapa* novoVizinho);
	void unlink(NoMapa* vizinho);
	int getCusto();
	void setCusto(int custo);
	int getDistFinal();
	void setDistFinal(int dist);
	void setInfo(void* info);
	void* getInfo();
	void abreVizinhos(Lista &lst);
};