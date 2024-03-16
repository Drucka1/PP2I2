#include "../../include/game/player.h"

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
