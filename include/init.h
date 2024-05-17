#ifndef INIT_H
#define INIT_H

#include "struct.h"

int numLevelFromChar(char* filename);
void dimMap(char* nomFichier, int* rows, int* cols);
Map* FileToMap(char *nomFichier,SDL_Rect* posPlayer, SDL_Texture** textures);
void MapToFile(Map* map,int posMapX, int posMapY);
SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]);
Entity* getPlayerStatus(int* current_level);
void tp(Map* *map,int i, int j,Entity* player,SDL_Texture** textures);
void posInitPlayerLevel(char *nomFichier, int* offset_player_x, int* offset_player_y);

#endif