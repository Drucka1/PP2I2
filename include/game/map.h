#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "interact.h"

Object *loadObject(char *tag, int i, int j, int facing, SDL_Renderer *renderer);
Cell *loadCell(int i, int j, SDL_Renderer *renderer);
Map *loadMap(SDL_Renderer *renderer);

void freeObject(Object *object);
void freeCell(Cell *cell);
void freeMap(Map *map);

void renderObject(Object *object, SDL_Renderer* renderer);
void renderCell(Cell *cell, SDL_Renderer* renderer);
void renderMap(Map *map, Entity *player, SDL_Renderer* renderer);
   
#endif
