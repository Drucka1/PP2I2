#include "../include/structs.h"

SDL_Texture *loadTexture(char *tag, SDL_Renderer *renderer)
{
  char path[100];
  snprintf(path, sizeof(path), "assets/%s.bmp", tag);

  SDL_Surface* surface = SDL_LoadBMP(path);
  if (!surface) {
    printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
    return NULL;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

