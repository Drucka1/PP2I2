#ifndef AUX_H
#define AUX_H

#include "../include/list.h"
#include <stdlib.h>

int sommeMatrice(int** matrice, int rows, int cols);
void freeTerrain(List* **terrain,int rows, int cols);
void freeMap(Map* map);
void freeSprites(SDL_Texture** textures,int nbSprites);

#endif