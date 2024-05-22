#ifndef __MAP_H__
#define __MAP_H__

#include "objects.h"

Object *initObject(Index index);
Cell *initCell(Index index);
Map *loadMap(int room, SDL_Texture **textures);
Map **loadRooms(SDL_Texture **textures);

#endif // __MAP_H__
