#ifndef __PLAYER__
#define __PLAYER__

#include "map.h"

#define RENDER_I(object)                                                       \
  ((object)->index.i - RENDER_MIN_I(player, map) + (map)->offset.i)
#define RENDER_J(object)                                                       \
  ((object)->index.j - RENDER_MIN_J(player, map) + (map)->offset.j)

Entity *loadPlayer(Index index, SDL_Renderer *renderer);
void freePlayer(Entity *player);

int renderMinI(Entity *player, Map *map);
int renderMinJ(Entity *player, Map *map);
int renderMaxJ(Entity *player, Map *map);
int renderMaxI(Entity *player, Map *map);
int renderI(Object *object, Map *map, Entity *player);
int renderJ(Object *object, Map *map, Entity *player);

void moveObjectBuffer(Map *map, Object *object, Entity *player);
void moveCellBuffer(Map *map, Cell *cell, Entity *player);
void moveMapBuffer(Map *map, Entity *player);

void movePlayer(Entity *player, Map *map, Index dest);
void moveRight(Entity *player, Map *map);
void moveUp(Entity *player, Map *map);
void moveLeft(Entity *player, Map *map);
void moveDown(Entity *player, Map *map);
void teleport(int room, Index spawnIndex, Entity *player, Map **map,
              Map **rooms);
#endif // __PLAYER__
