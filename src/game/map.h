#ifndef __MAP_H__
#define __MAP_H__

#include "objects.h"

// Vérification des indices
#define VALID_INDEX(index)                                                     \
  ((((index.i) < 0) || ((index.j) < 0) || ((index.i) >= map->dimensions.i) ||  \
    ((index.j) >= map->dimensions.j))                                          \
       ? (0)                                                                   \
       : (1))

Object *initObject(Index index);
Cell *initCell(Index index);
Map *loadMap(int room, SDL_Texture **textures);
Map **loadRooms(SDL_Texture **textures);

#endif // __MAP_H__
