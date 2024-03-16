#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

Entity *loadPlayer(int i, int j, SDL_Renderer *renderer);
void freePlayer(Entity *player);

void renderPlayer(Entity *player, SDL_Renderer *renderer);

#endif
