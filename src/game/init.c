#include "../../include/game/init.h"
#include <stdio.h>

void launchGame(SDL_Renderer *renderer)
{
  int row = 11;
  int column = 20;

  createMap(row, column);
  char filePath[100];
  sprintf(filePath, "assets/map/test_%d_%d.txt", row, column);

  Map *map = loadMap(filePath, renderer);
  Entity *player = loadPlayer(map->i_spawn, map->j_spawn, map, renderer);

  int quit = 0;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      // Intéraction entre joueur et jeu
      play(event, player, map);
    }
    // Affichage du jeu
    render(renderer, map, player);
  }
  // Libération de la carte et du joueur
  freeGame(player, map);
}

void render(SDL_Renderer *renderer, Map *map, Entity *player)
{
  // (Ré)initialisation de rendu 
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Affichage de la carte et du joueur
  renderMap(map, player, renderer);
  renderPlayer(player, renderer);

  // Affichage dans le moteur de rendu
  SDL_RenderPresent(renderer);
  SDL_Delay(10);
}

void freeGame(Entity *player, Map *map)
{
  freePlayer(player);
  freeMap(map);
}

