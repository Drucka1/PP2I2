#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "../structs.h"
#include "../utils.h"

void pickItem(Entity *player, Map *map, int objectType);
void openDoor(Index doorIndex, Entity *player, Map *map, Map **rooms);
void switchLever(Index leverIndex, Map *map, Map **rooms);

#endif // __OBJECTS_H__
