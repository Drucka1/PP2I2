#include "player.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>

Entity *loadPlayer(Index index, SDL_Renderer *renderer) {
  Entity *player = malloc(sizeof(Entity));

  player->index = index;
  player->prevIndex = index;

  player->facing = RIGHT;
  player->moving = 0;

  player->buffer = malloc(sizeof(SDL_Rect));
  player->buffer->x = indexToPixel(index.j);
  player->buffer->y = indexToPixel(index.i);
  player->buffer->w = TILE_SIZE;
  player->buffer->h = TILE_SIZE;

  player->texture = getTexture("player", renderer);
  player->textureBuffer = malloc(sizeof(SDL_Rect));

  SDL_QueryTexture(player->texture, NULL, NULL, &player->textureBuffer->w,
                   &player->textureBuffer->h);

  player->textureBuffer->x = 0;
  player->textureBuffer->y = 0;
  player->textureBuffer->w /= 4;
  player->textureBuffer->h /= 4;

  player->inventory = NULL;

  player->status = (Status){false};

  return player;
}

void freePlayer(Entity *player) {
  free(player->buffer);
  free(player->textureBuffer);
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
  object->buffer->x = indexToPixel(renderJ(object, map, player));
  object->buffer->y = indexToPixel(renderI(object, map, player));
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

    Index playerIndex = {
        player->index.i - renderMinI(player, map) + map->offset.i,
        player->index.j - renderMinJ(player, map) + map->offset.j};

    player->buffer->x = indexToPixel(playerIndex.j);
    player->buffer->y = indexToPixel(playerIndex.i);

    player->moving += 1;
    player->moving %= 4;

    moveMapBuffer(map, player);
  }
}

void move(Entity *player, Map *map, int direction) {
  Index dest = player->index;
  switch (direction) {
  case RIGHT:
    dest.j++;
    player->facing = RIGHT;
    break;
  case UP:
    dest.i--;
    player->facing = UP;
    break;
  case LEFT:
    dest.j--;
    player->facing = LEFT;
    break;
  case DOWN:
    dest.i++;
    player->facing = DOWN;
    break;
  }
  movePlayer(player, map, dest);
}

void teleport(int room, Index destIndex, Entity *player, Map **map,
              Map **rooms) {
  player->prevIndex = player->index;
  player->index = destIndex;
  *map = rooms[room];
  movePlayer(player, *map, destIndex);
}
