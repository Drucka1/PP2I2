#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "interact.h"

#include <time.h>
#define RANDOM_INT(MIN, MAX) (MIN + (rand())% (MAX - MIN + 1))

Object *loadObject(char *tag, int i, int j, int facing, SDL_Renderer *renderer);
Cell *loadCell(int i, int j, SDL_Renderer *renderer);
Map *loadMap(char *filePath, SDL_Renderer *renderer);

void addObject(Object *object, int steppable, Cell **cell);
void createMap(int numberRows, int numberColumns);

void freeObject(Object *object);
void freeCell(Cell *cell);
void freeMap(Map *map);

void renderObject(Object *object, SDL_Renderer* renderer);
void renderCell(Cell *cell, SDL_Renderer* renderer);
void renderMap(Map *map, Entity *player, SDL_Renderer* renderer);
   
#endif
