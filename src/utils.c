#include "../include/utils.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    exit(-1);
  }

  (*window) = SDL_CreateWindow("Grid Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!(*window)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window: %s", SDL_GetError());
    exit(-1);
  }

  (*renderer) = SDL_CreateRenderer((*window), -1, SDL_RENDERER_ACCELERATED);
  if (!(*renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize renderer: %s", SDL_GetError());
    exit(-1);
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
    exit(-1);
  }
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

char* getPath(bool defaultPath, int lvl) {
  char* buffer = malloc(sizeof(char)*50);
  if (defaultPath) sprintf(buffer, "assets/level/default/level%d.txt", lvl);
  else sprintf(buffer, "assets/level/save/level%d.txt", lvl);
  return buffer;
}
