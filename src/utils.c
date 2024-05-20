#include "../include/utils.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    exit(-1);
  }

  (*window) = SDL_CreateWindow("Grid Drawing", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                               WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!(*window)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window: %s",
                 SDL_GetError());
    SDL_Quit();
    exit(-1);
  }

  (*renderer) = SDL_CreateRenderer((*window), -1, SDL_RENDERER_ACCELERATED);
  if (!(*renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize renderer: %s", SDL_GetError());
    SDL_DestroyWindow(*window);
    SDL_Quit();
    exit(-1);
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "SDL_image could not initialize! SDL_image Error: %s",
                 IMG_GetError());
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_Quit();
    exit(-1);
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n",
            TTF_GetError());
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);    
    SDL_Quit();
  }
}


void quitSDL(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
