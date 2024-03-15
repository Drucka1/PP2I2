#include "../../include/game/init.h"

void launchGame(SDL_Renderer *renderer)
{
  Map *map = loadMap(renderer);
  // initPlayer(map, renderer);

  int quit = 0;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }
    renderMap(map, renderer);
  }
  
  freeMap(map); 
}
