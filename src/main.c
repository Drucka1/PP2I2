#include "game/game.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  SDL_Renderer *renderer = NULL;
  SDL_Window *window = NULL;
  TTF_Font * font = NULL;
  initSDL(&window, &renderer,&font);

  launchGame(renderer,font);

  quitSDL(window, renderer,font);
  return 0;
}
