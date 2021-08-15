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

#include "Mapa.hpp"
#include "Graphics.h"
#include "Audio.h"
#include "string.h"
#include "stdio.h"
#include "load.h"
#include "Lista.hpp"
#include "pos.h"
#include "LeDoces.h"

#define WINDOW_W 1024
#define WINDOW_H 768

#define INITIAL_POS_X (-24*8)
#define INITIAL_POS_Y (-18*8)

int _mapaW;
int _mapaH;
int _posX;
int _posY;

#define ZOOM 2

Graphics graphics;
Audio _bckMusic, _wildEncounter;
Image _imgLoading, _imgGrass, _imgTree, _imgGalhos, _imgLobo, _imgCasa1, _imgCasa2, _imgVisitado, _imgCaminho, _imgVermelho[4][4];
Lista _caminhosEq, _visitados, _caminhoChapeuzinho, _precosClareiras, _docesClareiras, _custosClareiras;

typedef enum eDirec {
	BAIXO,
	ESQUERDA,
	DIREITA,
	CIMA
} Direc;

typedef struct sChapeu {
	bool andando, visivel;
	Pos pos;
	Direc dir;
	int speed;
	int indexCaminho;
	bool curva;
	int imgIndex;
	float timer;
} Chapeuzinho;

void loadImages();
void teclado(int key, int state, int x, int y);
void mainLoop();

int main() {

	graphics.CreateMainWindow(WINDOW_W, WINDOW_H, "Chapeuzinho Vermelho");
	loadImages();
	_bckMusic.LoadAudio("Audio\\PelaEstradaAfora.mp3");
	_wildEncounter.LoadAudio("Audio\\WildEncounter.mp3");
	graphics.SetKeyboardInput(teclado);
	graphics.SetBackgroundColor(0, 102, 51);
	graphics.SetMainLoop(mainLoop);
	graphics.StartMainLoop();

	return 0;
}

void mainLoop() {
	float timer = graphics.GetElapsedTime();

	static float tempoLobo = 0;
	static NoMapa* ultimaClareira = NULL;

	static int trigger = 2;
	static Mapa mapa;
	static int inicialX, inicialY, finalX, finalY;
	static Chapeuzinho chapeuzinho;

	static bool wildEncounter = false;
	static bool encounterTrigger = true;
	static bool mostraCaminhos = true;
	static bool finalizado = false;
	static bool clareiraTrigger = true;
	static float custoFinal;
	static int clareirasPassadas = 0;

	float custoTotal = 0;

	if (trigger == 2) {
		graphics.DrawImage2D(0, 0, WINDOW_W, WINDOW_H, _imgLoading);
		trigger = 1;
		return;
	} else if (trigger == 1) {

		mapa.loadMapa("Mapa.txt");
		//mapa.printDistancias("antes.txt");
		mapa.calculaDistancias();
		mapa.printDistancias("Distancias.txt");
		mapa.calculaRota(_visitados, _caminhosEq);
		mapa.calculaCaminhoFinal(_caminhoChapeuzinho);
		leDoces("melhores.txt", _precosClareiras, _docesClareiras);

		_mapaW = mapa.getWidth();
		_mapaH = mapa.getHeight();
		inicialX = mapa.getInicialX();
		inicialY = mapa.getInicialY();
		finalX = mapa.getFinalX();
		finalY = mapa.getFinalY();
		_posX = INITIAL_POS_X;
		_posY = INITIAL_POS_Y;

		chapeuzinho.pos.x = chapeuzinho.pos.y = 0;
		chapeuzinho.speed = 2;
		chapeuzinho.andando = false;
		chapeuzinho.indexCaminho = 1;
		chapeuzinho.curva = true;
		chapeuzinho.imgIndex = 0;
		chapeuzinho.timer = 0;
		chapeuzinho.visivel = false;

		trigger = 0;
		return;
	}

	if (!_bckMusic.IsPlaying() && !wildEncounter) {
		_bckMusic.Stop();
		_bckMusic.Play();
	}

	graphics.SetColor(1, 0, 0);

	int sqr_w = WINDOW_W/_mapaW;
	int sqr_h = WINDOW_H/_mapaH;

	sqr_w *= ZOOM;
	sqr_h *= ZOOM;

	for (int i = _mapaH-1; i >= 0; i--)
		for (int j = 0; j < _mapaW; j++)
			graphics.DrawImage2D(j*sqr_w-_posX, i*sqr_h-_posY, sqr_w, sqr_h, _imgGrass);

	if (ultimaClareira != _caminhoChapeuzinho[chapeuzinho.indexCaminho] && mapa.getClareiras().busca(_caminhoChapeuzinho[chapeuzinho.indexCaminho]) != -1) {
		chapeuzinho.andando = false;
		tempoLobo += timer;

		if (encounterTrigger) {
			encounterTrigger = false;
			wildEncounter = true;
			_wildEncounter.Play();
			_bckMusic.Pause();
		}

		/*graphics.DrawText2D(10, (_mapaH*sqr_h)-50, "Clareira %d: Fome do lobo: %.2f", clareirasPassadas, *(float*)_precosClareiras[clareirasPassadas-1]);

		Lista* umaClareira = (Lista*)_docesClareiras[clareirasPassadas-1];

		graphics.DrawText2D(10, (_mapaH*sqr_h)-70, "Doces utilizados: ");

		float somaDoces = 0;
		for (int i = 0; i < umaClareira->getSize(); i++) {
			float doce = *(float*)(*umaClareira)[i];
			graphics.DrawText2D(10+100*i, (_mapaH*sqr_h)-90, "%.2f", doce);
			somaDoces += doce;
		}

		if (somaDoces == 0)
			{printf("Erro em main: clareira sem doces\n\n");	exit(1);}

		graphics.DrawText2D(10, 10, "Custo total da clareira: %.2f", somaDoces);

		if (clareiraTrigger) {
			clareiraTrigger = false;
			float* novoCustoClar = new float;
			*novoCustoClar = somaDoces;
			_custosClareiras.push(novoCustoClar);
		}*/
		

		if (tempoLobo >= 5) {
			tempoLobo = 0;
			chapeuzinho.andando = true;
			encounterTrigger = true;
			clareiraTrigger = true;
			wildEncounter = false;
			_wildEncounter.Stop();
			_bckMusic.Play();
			ultimaClareira = (NoMapa*)_caminhoChapeuzinho[chapeuzinho.indexCaminho];
		}
	}

	if (chapeuzinho.andando) {

		for (int i = 0; i < _caminhoChapeuzinho.getSize() && i < chapeuzinho.indexCaminho+1; i++) {
			custoTotal += ((NoMapa*)_caminhoChapeuzinho[i])->getCusto();
		}
		for (int i = 0; i < _custosClareiras.getSize(); i++) {
			custoTotal += *(float*)_custosClareiras[i];
		}

		if (chapeuzinho.curva) {
			NoMapa* noCorrente = (NoMapa*)_caminhoChapeuzinho[chapeuzinho.indexCaminho];
			chapeuzinho.pos = *(Pos*)noCorrente->getInfo();
			chapeuzinho.pos.x *= sqr_w;
			chapeuzinho.pos.y *= sqr_h;
			chapeuzinho.indexCaminho += 1;
			NoMapa* proxNo = (NoMapa*)_caminhoChapeuzinho[chapeuzinho.indexCaminho];
			Pos newPos = *(Pos*)proxNo->getInfo();
			if (newPos.x == mapa.getFinalX() && newPos.y == mapa.getFinalY()) {
				chapeuzinho.andando = false;
				chapeuzinho.visivel = false;
				finalizado = true;
				custoTotal += ((NoMapa*)_caminhoChapeuzinho[chapeuzinho.indexCaminho])->getCusto();
				custoFinal = custoTotal;
			}
			else {
				newPos.x *= sqr_w;
				newPos.y *= sqr_h;
				Direc newDir;
				if (newPos.x > chapeuzinho.pos.x)
					newDir = DIREITA;
				else if (newPos.x < chapeuzinho.pos.x)
					newDir = ESQUERDA;
				else if (newPos.y > chapeuzinho.pos.y)
					newDir = BAIXO;
				else if (newPos.y < chapeuzinho.pos.y)
					newDir = CIMA;
				if (chapeuzinho.dir != newDir) {
					chapeuzinho.imgIndex = 0;
					chapeuzinho.dir = newDir;
				}
				//graphics.DrawImage2D(chapeuzinho.pos.x-_posX, (((_mapaH-1)*sqr_h)-chapeuzinho.pos.y)-_posY, sqr_w, 1.5*sqr_h, _imgVermelho[chapeuzinho.dir][chapeuzinho.imgIndex]);
			}
			chapeuzinho.curva = false;
		} else {
			//graphics.DrawImage2D(chapeuzinho.pos.x-_posX, (((_mapaH-1)*sqr_h)-chapeuzinho.pos.y)-_posY, sqr_w, 1.5*sqr_h, _imgVermelho[chapeuzinho.dir][chapeuzinho.imgIndex]);
			NoMapa* proxNo = (NoMapa*)_caminhoChapeuzinho[chapeuzinho.indexCaminho];
			Pos proxPos = *(Pos*)proxNo->getInfo();
			proxPos.x *= sqr_w;
			proxPos.y *= sqr_h;
			switch(chapeuzinho.dir) {
			case CIMA:
				chapeuzinho.pos.y -= chapeuzinho.speed;
				_posY += chapeuzinho.speed;
				if (chapeuzinho.pos.y <= proxPos.y)
					chapeuzinho.curva = true;
				break;
			case BAIXO:
				chapeuzinho.pos.y += chapeuzinho.speed;
				_posY -= chapeuzinho.speed;
				if (chapeuzinho.pos.y >= proxPos.y)
					chapeuzinho.curva = true;
				break;
			case ESQUERDA:
				chapeuzinho.pos.x -= chapeuzinho.speed;
				_posX -= chapeuzinho.speed;
				if (chapeuzinho.pos.x <= proxPos.x)
					chapeuzinho.curva = true;
				break;
			case DIREITA:
				chapeuzinho.pos.x += chapeuzinho.speed;
				_posX += chapeuzinho.speed;
				if (chapeuzinho.pos.x >= proxPos.x)
					chapeuzinho.curva = true;
				break;
			}

			chapeuzinho.timer += timer;
			if (chapeuzinho.timer >= 0.15/chapeuzinho.speed) {
				chapeuzinho.timer = 0;
				chapeuzinho.imgIndex++;
				chapeuzinho.imgIndex %= 4;
			}
		}
	}

	bool d = 1;
	for (int i = _mapaH-1; i >= 0; i--)
		for (int j = 0; j < _mapaW; j++) {

			switch(mapa.getCustoNo(_mapaH-i-1, j)) {
			case 5:
				graphics.DrawImage2D(j*sqr_w-_posX, i*sqr_h-_posY, sqr_w, sqr_h, _imgGalhos);	break;
			case 200:
				graphics.DrawImage2D(j*sqr_w-_posX, i*sqr_h-_posY, sqr_w, sqr_h*2, _imgTree);	break;
			//	graphics.DrawImage2D(8*sqr_w, 8*sqr_h, sqr_w, 1.5*sqr_h, _imgVermelho[ESQUERDA][0]);
			//	graphics.DrawImage2D(12*sqr_w, 8*sqr_h, sqr_w, 1.5*sqr_h, _imgLobo);
			}


			if (d== 1 && chapeuzinho.visivel && ((_mapaH-1)*sqr_h)-chapeuzinho.pos.y > i*sqr_h) {
				int indiceLobo = mapa.getClareiras().busca(_caminhoChapeuzinho[chapeuzinho.indexCaminho]);
				Pos* posClar;
				int clarX;
				int clarY;
				bool lobo = false;
				if (!chapeuzinho.andando && indiceLobo != -1) {
					posClar = (Pos*)(((NoMapa*)mapa.getClareiras()[indiceLobo])->getInfo());
					clarX = posClar->x;
					clarY = posClar->y;
					lobo = true;
					//graphics.DrawImage2D(clarX*sqr_w-_posX, (_mapaH-clarY-1)*sqr_h-_posY, sqr_w, 1.5*sqr_h, _imgLobo);
				}
				if (lobo && (_mapaH-clarY-1)*sqr_h > ((_mapaH-1)*sqr_h)-chapeuzinho.pos.y)
					graphics.DrawImage2D(clarX*sqr_w-_posX, (_mapaH-clarY-1)*sqr_h-_posY, sqr_w, 1.5*sqr_h, _imgLobo);
				graphics.DrawImage2D(chapeuzinho.pos.x-_posX, (((_mapaH-1)*sqr_h)-chapeuzinho.pos.y)-_posY, sqr_w, 1.5*sqr_h, _imgVermelho[chapeuzinho.dir][chapeuzinho.imgIndex]);
				if (lobo && (_mapaH-clarY-1)*sqr_h < ((_mapaH-1)*sqr_h)-chapeuzinho.pos.y)
					graphics.DrawImage2D(clarX*sqr_w-_posX, (_mapaH-clarY-1)*sqr_h-_posY, sqr_w, 1.5*sqr_h, _imgLobo);
				d=0;
			}
		}

	graphics.DrawImage2D((inicialX-1)*sqr_w-_posX, (_mapaH-inicialY-1)*sqr_h-_posY, sqr_w*3, sqr_h*3, _imgCasa1);
	graphics.DrawImage2D((finalX-1)*sqr_w-_posX, (_mapaH-finalY-1)*sqr_h-_posY, sqr_w*3, sqr_h*3, _imgCasa2);

	static float indiceCaminho = 0;
	for (int i = 0; i < _caminhosEq.getSize() && i < indiceCaminho && mostraCaminhos; i++) {
			NoMapa* no = ((NoMapa*)_caminhosEq[i]);
			Pos* a = (Pos*)(no->getInfo());
			if (i >= indiceCaminho-1) {
				_posX = 2*INITIAL_POS_X + a->x*sqr_w;
				_posY = 2*INITIAL_POS_Y + (_mapaH - a->y)*sqr_h;
			}
			graphics.DrawImage2D(a->x*sqr_w-_posX, (_mapaH-a->y-1)*sqr_h-_posY, sqr_w, sqr_h, _imgCaminho);
			custoTotal += no->getCusto();
		}
	if (mostraCaminhos)
		indiceCaminho += 0.1;
	if (mostraCaminhos && indiceCaminho > _caminhosEq.getSize()) {
		//Sleep(5000);
		mostraCaminhos = false;
		chapeuzinho.andando = true;
		chapeuzinho.visivel = true;
		_posX = INITIAL_POS_X;
		_posY = INITIAL_POS_Y;
		indiceCaminho = 0;
	}

	static float indiceVisita = 0;
	for (int i = 0; i < _visitados.getSize() && i < indiceVisita && mostraCaminhos; i++) {
			Pos* a = (Pos*)(((NoMapa*)_visitados[i])->getInfo());
			graphics.DrawImage2D(a->x*sqr_w-_posX, (_mapaH-a->y-1)*sqr_h-_posY, sqr_w, sqr_h, _imgVisitado);
		}

	if (mostraCaminhos)
		indiceVisita += 0.17;
	if (mostraCaminhos && indiceVisita > _visitados.getSize())
		indiceVisita = 0;

	if (custoTotal)
		graphics.DrawText2D(10, 10, "Custo total ate o momento: %.2f", custoTotal);
	else if (finalizado)
		graphics.DrawText2D(10, 10, "Custo total final: %.2f", custoFinal);

		if (ultimaClareira != _caminhoChapeuzinho[chapeuzinho.indexCaminho] && mapa.getClareiras().busca(_caminhoChapeuzinho[chapeuzinho.indexCaminho]) != -1) {

			if (clareiraTrigger)
				clareirasPassadas++;

			graphics.DrawText2D(10, (_mapaH*sqr_h)/ZOOM, "Clareira %d: Fome do lobo: %.2f", clareirasPassadas, *(float*)_precosClareiras[clareirasPassadas-1]);

			Lista* umaClareira = (Lista*)_docesClareiras[clareirasPassadas-1];

			graphics.DrawText2D(10, (_mapaH*sqr_h)/ZOOM-20, "Doces utilizados: ");

			float somaDoces = 0;
			for (int i = 0; i < umaClareira->getSize(); i++) {
				float doce = *(float*)(*umaClareira)[i];
				graphics.DrawText2D(10+100*i, (_mapaH*sqr_h)/ZOOM-40, "%.2f", doce);
				somaDoces += doce;
			}

			if (somaDoces == 0)
				{printf("Erro em main: clareira sem doces\n\n");	exit(1);}

			graphics.DrawText2D(10, 10, "Custo total da clareira: %.2f", (*(float*)_precosClareiras[clareirasPassadas-1])/somaDoces);

			if (clareiraTrigger) {
				clareiraTrigger = false;
				float* novoCustoClar = new float;
				*novoCustoClar = (*(float*)_precosClareiras[clareirasPassadas-1])/somaDoces;
				_custosClareiras.push(novoCustoClar);
			}
		}
}

void teclado(int key, int state, int x, int y) {

	if (state == KEY_STATE_UP)
		return;
	if (key == KEY_RIGHT)
		_posX += 50;
	else if (key == KEY_LEFT)
		_posX -= 50;
	else if (key == KEY_DOWN)
		_posY -=50;
	else if (key == KEY_UP)
		_posY += 50;
}

void loadImages() {
	imgLoad(_imgLoading, "carregando");
	imgLoad(_imgGrass, "grass");
	imgLoad(_imgTree, "tree");
	imgLoad(_imgGalhos, "galhos");
	imgLoad(_imgLobo, "lobo");
	imgLoad(_imgCasa1, "casa1");
	imgLoad(_imgCasa2, "casa2");
	imgLoad(_imgVisitado, "visitado");
	imgLoad(_imgCaminho, "caminho");
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			imgLoad(_imgVermelho[i][j], "vermelho\\vermelho", 10*(i+1)+(j+1));
}