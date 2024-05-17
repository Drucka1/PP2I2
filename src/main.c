#include "game/game.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  SDL_Renderer *renderer = NULL;
  SDL_Window *window = NULL;

  initSDL(&window, &renderer);

  launchGame(renderer);

  quitSDL(window, renderer);
  return 0;
}
