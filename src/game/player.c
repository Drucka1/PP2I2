#include "../../include/game/player.h"

Entity *loadPlayer(int i, int j, Map *map, SDL_Renderer *renderer) 
{
  Entity *player = malloc(sizeof(Entity));

  player->tag = "player";
  player->facing = FACING_RIGHT;

  player->buffer = malloc(sizeof(SDL_Rect));
  player->buffer->w = TILE_SIZE;
  player->buffer->h = TILE_SIZE;

  player->texture = loadTexture(player->tag, renderer);
    
  movePlayer(player, map, i, j);
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

void moveObjectBuffer(Object *object, Map *map, Entity *player)
{
  object->buffer->x = indexToPixel(RENDER_J(object)); 
  object->buffer->y = indexToPixel(RENDER_I(object)); 
}

void moveCellBuffer(Cell *cell, Map *map, Entity *player)
{
  for (size_t k = 0; k < cell->numberObjects; k++) {
    moveObjectBuffer(cell->objects[k], map, player);
  }
}

void moveMapBuffer(Map *map, Entity *player)
{
  for (int i = RENDER_MIN_I; i < RENDER_MAX_I; i++) {
    for (int j = RENDER_MIN_J; j < RENDER_MAX_J; j++) {
      moveCellBuffer(cell(i, j), map, player);
    }
  }
}

void movePlayer(Entity *player, Map *map, int i_dest, int j_dest) 
{
  // Déplacement que si c'est possible
  if (VALID_INDEX(i_dest, j_dest) && cell(i_dest, j_dest)->steppable) {
    player->i = i_dest;
    player->j = j_dest;

    // Décalage de la carte selon la position du joueur
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

