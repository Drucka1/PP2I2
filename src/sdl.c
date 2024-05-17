#include "sdl.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    exit(-1);
  }

  (*window) =
      SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!(*window)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window: %s",
                 SDL_GetError());
    exit(-1);
  }

  (*renderer) = SDL_CreateRenderer((*window), -1, SDL_RENDERER_ACCELERATED);
  if (!(*renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize renderer: %s", SDL_GetError());
    exit(-1);
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "SDL_image could not initialize! SDL_image Error: %s",
                 IMG_GetError());
    exit(-1);
  }
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

SDL_Texture *getTexture(char *tag, SDL_Renderer *renderer) {
  char path[64];
  snprintf(path, sizeof(path), "assets/sprites/%s.png", tag);
  SDL_Surface *surface = IMG_Load(path);
  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load image: %s",
                 IMG_GetError());
    exit(-1);
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s",
                 SDL_GetError());
    exit(-1);
  }

  SDL_FreeSurface(surface);
  return texture;
}
