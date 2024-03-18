#include "../../include/game/player.h"
#include <stdio.h>

Entity *loadPlayer(int i, int j, SDL_Renderer *renderer) 
{
  Entity *player = malloc(sizeof(Entity));

  player->tag = "player";
  player->i = i;
  player->j = j;
  player->facing = FACING_RIGHT;

  player->buffer = malloc(sizeof(SDL_Rect));
  player->buffer->x = indexToPixel(j); 
  player->buffer->y = indexToPixel(i); 
  player->buffer->w = TILE_SIZE;
  player->buffer->h = TILE_SIZE;

  player->texture = loadTexture(player->tag, renderer);
    
  return player;
}

void freePlayer(Entity *player)
{
  free(player->buffer);
  SDL_DestroyTexture(player->texture);
  free(player);
}

void renderPlayer(Entity *player, SDL_Renderer *renderer)
{
  SDL_RenderCopy(renderer, player->texture, NULL, player->buffer);
}

void moveObjectBuffer(Object *object, Entity *player)
{
  object->buffer->x = indexToPixel(RENDER_J(object)); 
  object->buffer->y = indexToPixel(RENDER_I(object)); 
}

void moveCellBuffer(Cell *cell, Entity *player)
{
  for (size_t k = 0; k < cell->numberObjects; k++) {
    moveObjectBuffer(cell->objects[k], player);
  }
}

void moveMapBuffer(Map *map, Entity *player)
{
  for (int i = RENDER_MIN_I; i < RENDER_MAX_I; i++) {
    for (int j = RENDER_MIN_J; j < RENDER_MAX_J; j++) {
      moveCellBuffer(cell(i, j), player);
    }
  }
}

void movePlayer(Entity *player, Map *map, int i_dest, int j_dest) 
{
  if (VALID_INDEX(i_dest, j_dest)) {
    player->i = i_dest;
    player->j = j_dest;

    moveMapBuffer(map, player);

    player->buffer->x = indexToPixel(RENDER_J(player));
    player->buffer->y = indexToPixel(RENDER_I(player));
  }
}

void moveRight(Entity *player, Map *map) 
{
  movePlayer(player, map, player->i, player->j + 1);
  player->facing = FACING_RIGHT;
}
void moveUp(Entity *player, Map *map) 
{
  movePlayer(player, map, player->i - 1, player->j);
  player->facing = FACING_UP;
}
void moveLeft(Entity *player, Map *map) 
{
  movePlayer(player, map, player->i, player->j - 1);
  player->facing = FACING_LEFT;
}
void moveDown(Entity *player, Map *map) 
{
  movePlayer(player, map, player->i + 1, player->j);
  player->facing = FACING_DOWN;
}

