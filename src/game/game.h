#ifndef __GAME__
#define __GAME__

#include "../utils.h"
#include "interact.h"

void launchGame(SDL_Renderer *renderer);

void update(Entity *player, Map **map, Map **rooms);
void play(SDL_Event event, Entity *player, Map *map);

void renderEntity(Entity *entity, SDL_Renderer *renderer);
void renderObject(Object *object, SDL_Renderer *renderer);
void renderCell(Cell *cell, SDL_Renderer *renderer);
void renderMap(Map *map, Entity *player, SDL_Renderer *renderer);
void render(SDL_Renderer *renderer, Map *map, Entity *player);

void freeGame(Entity *player, SDL_Texture **textures, Map **rooms);
#endif
