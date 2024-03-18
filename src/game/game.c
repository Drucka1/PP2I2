#include "../../include/game/game.h"

void play(SDL_Event event, Entity *player, Map* map)
{
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
      // Déplacement du joueur
      case SDLK_UP:
      case SDLK_z:
        moveUp(player, map);
        break;
      case SDLK_DOWN:
      case SDLK_s:
        moveDown(player, map);
        break;
      case SDLK_LEFT:
      case SDLK_q:
        moveLeft(player, map);
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        moveRight(player, map);
        break;

      // Intéraction 
      case SDLK_SPACE:
        interact(player, map);
        break;
      default:
        break;
    }
  }
}

