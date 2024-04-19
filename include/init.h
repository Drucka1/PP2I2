#ifndef INIT_H
#define INIT_H

#include "struct.h"

void dimMap(char* nomFichier, int* rows, int* cols);
Map* FileToMap(char *nomFichier,int pos_x, int pos_y, SDL_Texture** textures);
SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]);

#endif