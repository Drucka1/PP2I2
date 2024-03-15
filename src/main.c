#include "../include/utils.h"
#include "../include/game/init.h"
#include <stdbool.h>

int main(int argc, char* args[]) {

  (void) argc;
  (void) args;

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  initSDL(&window, &renderer);

  launchGame(renderer);

  quitSDL(window, renderer);
  return 0;
}

