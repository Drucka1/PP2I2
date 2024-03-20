#include "../include/utils.h"

int main(int argc, char* args[]) 
{
  // Pour éviter les messages d'erreurs puisqu'on utilise pas encore les arguments de main
  (void) argc;
  (void) args;

  // Initialisation d'SDL, de la fenêtre, du moteur de rendu et de sdl_image
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  initSDL(&window, &renderer);

  // Lancement du jeu
  launchGame(renderer);

  // Destruction du moteur de rendu, de la fenêtre et fermeture d'SDL
  quitSDL(window, renderer);
  return 0;
}

