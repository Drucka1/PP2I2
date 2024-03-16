#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>
#include "game.h"
#include "map.h"

void launchGame(SDL_Renderer *renderer);

void render(SDL_Renderer *renderer, Map *map, Entity *player);
void freeGame(Entity *player, Map *map);

#endif
