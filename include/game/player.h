#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "../structs.h"

// Fonctions min et max
#define MIN(i, j) ((i) < (j) ? (i) : (j))
#define MAX(i, j) ((i) > (j) ? (i) : (j))

// Indices des coins de la fenêtre dans la carte
#define RENDER_MIN_I (MIN((map->height - GRID_MAX_ROW), MAX(0, (player->i - GRID_MAX_ROW / 2))))
#define RENDER_MAX_I (MIN(MAX(GRID_MAX_ROW, (1 + player->i + GRID_MAX_ROW / 2)), map->height))
#define RENDER_MIN_J (MIN((map->width - GRID_MAX_COLUMN), MAX(0, (player->j - GRID_MAX_COLUMN / 2))))
#define RENDER_MAX_J (MIN(MAX(GRID_MAX_COLUMN, (1 + player->j + GRID_MAX_COLUMN / 2)), map->width))

// Incides des objets affichés dans la fenêtre 
#define RENDER_I(object) (object->i  - RENDER_MIN_I)
#define RENDER_J(object) (object->j  - RENDER_MIN_J)

Entity *loadPlayer(int i, int j, Map *map, SDL_Renderer *renderer);
void freePlayer(Entity *player);

void renderPlayer(Entity *player, SDL_Renderer *renderer);

void moveObjectBuffer(Object *object, Map *map, Entity *player);
void moveCellBuffer(Cell *cell, Map *map, Entity *player);
void moveMapBuffer(Map *map, Entity *player);

void movePlayer(Entity *player, Map *map, int iDest, int jDest);
void moveRight(Entity *player, Map *map);
void moveUp(Entity *player, Map *map);
void moveLeft(Entity *player, Map *map);
void moveDown(Entity *player, Map *map);

#endif
