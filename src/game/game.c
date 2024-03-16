#include "../../include/game/game.h"

void play(SDL_Event event, Entity *player)
{
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
      case SDLK_UP:
        moveUp(player);
        break;
      case SDLK_DOWN:
        moveDown(player);
        break;
      case SDLK_LEFT:
        moveLeft(player);
        break;
      case SDLK_RIGHT:
        moveRight(player);
        break;
      default:
        break;
    }
  }
}
