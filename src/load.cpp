#include "load.h"
#include "stdio.h"

void imgLoad(Image& img, char* name, int index) {
	int pathSize = strlen(IMG_PATH);
	pathSize += strlen(name);
	pathSize += 5; // ".png\0"
	int digitsSize = 1;
	int temp = index;
	while (temp >= 10) {
		digitsSize++;
		temp /= 10;
	};
	char* indexStr;
	if (index >= 0) {
		pathSize += digitsSize;
		indexStr = new char[digitsSize+1];
		if (!indexStr) {printf("Erro 1: falta de memoria\n\n");	exit(1);}
		itoa(index, indexStr, 10);
	}
	char* finalPath = new char[pathSize];
	if (!finalPath) {printf("Erro 2: falta de memoria\n\n");	exit(1);}
	strcpy(finalPath, IMG_PATH);
	strcat(finalPath, name);
	if (index >= 0)
		strcat(finalPath, indexStr);
	strcat(finalPath, ".png");
	img.LoadPNGImage(finalPath);
}