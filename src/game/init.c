#include "../../include/game/init.h"

void launchGame(SDL_Renderer *renderer)
{
  Map *map = loadMap(renderer);
  Entity *player = loadPlayer(0, 0, renderer);

  int quit = 0;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      play(event, player);
    }
    render(renderer, map, player);
  }
  
  freeGame(player, map);
}

void render(SDL_Renderer *renderer, Map *map, Entity *player)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  renderMap(map, renderer);
  renderPlayer(player, renderer);

  SDL_RenderPresent(renderer);
  SDL_Delay(10);
}

void freeGame(Entity *player, Map *map)
{
  freePlayer(player);
  freeMap(map);
}

