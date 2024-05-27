#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "../structs.h"
#include "../utils.h"

void pickItem(Entity *player, Map *map, int objectType);
void openDoor(Index doorIndex, Entity *player, Map *map, Map **rooms);
void openDoorc(Index doorIndex, Map *map, Map **rooms);
void openDooropen(Index doorIndex, Map *map, Map **rooms);
void switchLever(Index leverIndex, Map *map, Map **rooms);
void moveObject(int objecType, Index src, Map *map, Index dest);
void pushBlock(Index blockIndex, Entity *player, Map *map, Map **rooms);
void fleeingLever(Entity *player, Map *map);
void turnPressure(Index pressureIndex, Map *map, Map **rooms,bool on);
#endif // __OBJECTS_H__
