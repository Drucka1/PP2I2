#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

Entity *loadPlayer(int i, int j, SDL_Renderer *renderer);
void freePlayer(Entity *player);

void renderPlayer(Entity *player, SDL_Renderer *renderer);

void movePlayer(Entity *player, int iDest, int jDest);
void moveRight(Entity *player);
void moveUp(Entity *player);
void moveLeft(Entity *player);
void moveDown(Entity *player);


#endif
