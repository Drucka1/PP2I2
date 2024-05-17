#include "player.h"
#include <stdio.h>

Entity *loadPlayer(Index index, SDL_Renderer *renderer) {
  Entity *player = malloc(sizeof(Entity));

  player->index = index;
  player->prevIndex = index;

  player->facing = FACING_RIGHT;

  player->buffer = malloc(sizeof(SDL_Rect));
  player->buffer->x = indexToPixel(index.j);
  player->buffer->y = indexToPixel(index.i);
  player->buffer->w = TILE_SIZE;
  player->buffer->h = TILE_SIZE;

  player->texture = getTexture("player", renderer);

  player->inventory = NULL;

  return player;
}

void freePlayer(Entity *player) {
  free(player->buffer);
  SDL_DestroyTexture(player->texture);
  freeListObj(player->inventory);
  free(player);
}

int renderMinI(Entity *player, Map *map) {
  if (map->dimensions.i < GRID_MAX_ROW) {
    return 0;
  }
  return MIN(MAX(0, (player->index.i - (GRID_MAX_ROW) / 2)),
             (map->dimensions.i - GRID_MAX_ROW));
}

int renderMinJ(Entity *player, Map *map) {
  if (map->dimensions.j < GRID_MAX_COLUMN) {
    return 0;
  }
  return MIN(MAX(0, (player->index.j - (GRID_MAX_COLUMN) / 2)),
             (map->dimensions.j - GRID_MAX_COLUMN));
}

int renderMaxJ(Entity *player, Map *map) {
  if (map->dimensions.j < GRID_MAX_COLUMN) {
    return map->dimensions.j;
  }
  return MAX(
      MIN(map->dimensions.j, (1 + player->index.j + (GRID_MAX_COLUMN) / 2)),
      GRID_MAX_COLUMN);
}

int renderMaxI(Entity *player, Map *map) {
  if (map->dimensions.i < GRID_MAX_ROW) {
    return map->dimensions.i;
  }
  return MAX(MIN(map->dimensions.i, (1 + player->index.i + (GRID_MAX_ROW) / 2)),
             GRID_MAX_ROW);
}

int renderI(Object *object, Map *map, Entity *player) {
  return (object->index.i - renderMinI(player, map) + map->offset.i);
}

int renderJ(Object *object, Map *map, Entity *player) {
  return (object->index.j - renderMinJ(player, map) + map->offset.j);
}

void moveObjectBuffer(Map *map, Object *object, Entity *player) {
  object->buffer->x =
      indexToPixel(renderJ(object, map, player));
  object->buffer->y =
      indexToPixel(renderI(object, map, player));
}

void moveCellBuffer(Map *map, Cell *cell, Entity *player) {
  ListObj *current = cell->objects;
  while (current != NULL) {
    moveObjectBuffer(map, current->object, player);
    current = current->next;
  }
}

void moveMapBuffer(Map *map, Entity *player) {
  for (int i = renderMinI(player, map); i < renderMaxI(player, map); i++) {
    for (int j = renderMinJ(player, map); j < renderMaxJ(player, map); j++) {
      Index index = {i, j};
      if (VALID_INDEX(index)) {
        moveCellBuffer(map, cell(i, j), player);
      }
    }
  }
}

void movePlayer(Entity *player, Map *map, Index dest) {
  if (VALID_INDEX(dest) && cell(dest.i, dest.j)->steppable) {
    player->prevIndex = player->index;
    player->index = dest;

    Index playerIndex = {player->index.i - renderMinI(player, map) + map->offset.i, player->index.j - renderMinJ(player, map) + map->offset.j};

    player->buffer->x = indexToPixel(playerIndex.j);
    player->buffer->y = indexToPixel(playerIndex.i);
  }
}

void moveRight(Entity *player, Map *map) {
  Index dest = {player->index.i, player->index.j + 1};
  movePlayer(player, map, dest);
  player->facing = FACING_RIGHT;
}
void moveUp(Entity *player, Map *map) {
  Index dest = {player->index.i - 1, player->index.j};
  movePlayer(player, map, dest);
  player->facing = FACING_UP;
}
void moveLeft(Entity *player, Map *map) {
  Index dest = {player->index.i, player->index.j - 1};
  movePlayer(player, map, dest);
  player->facing = FACING_LEFT;
}
void moveDown(Entity *player, Map *map) {
  Index dest = {player->index.i + 1, player->index.j};
  movePlayer(player, map, dest);
  player->facing = FACING_DOWN;
}
