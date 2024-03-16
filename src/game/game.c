#include "../../include/game/game.h"

void play(SDL_Event event, Entity *player)
{
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
      case SDLK_UP:
      case SDLK_z:
        moveUp(player);
        break;
      case SDLK_DOWN:
      case SDLK_s:
        moveDown(player);
        break;
      case SDLK_LEFT:
      case SDLK_q:
        moveLeft(player);
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        moveRight(player);
        break;
      default:
        break;
    }
  }
}
