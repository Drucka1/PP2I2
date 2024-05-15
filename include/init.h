#ifndef INIT_H
#define INIT_H

#include "struct.h"

int numLevelFromChar(char* filename);
void dimMap(char* nomFichier, int* rows, int* cols);
Map* FileToMap(char *nomFichier,int pos_x, int pos_y, SDL_Texture** textures);
void MapToFile(Map* map);
SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]);
Entity* getPlayerStatus(int* current_level);

#endif