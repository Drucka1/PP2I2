#include "utils.h"

SDL_Texture **loadTextures(SDL_Renderer *renderer) {
  SDL_Texture **textures = malloc(sizeof(SDL_Texture *) * TEXTURE_COUNT);

  char *textureTags[TEXTURE_COUNT] = TEXTURE_TAGS;
  if (!textures) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate memory");
    exit(-1);
  }

  for (int i = 0; i < TEXTURE_COUNT; i++) {
    textures[i] = getTexture(textureTags[i], renderer);
  }

  return textures;
}

void screenshot(SDL_Renderer *renderer) {
  // Get the current time
  time_t currentTime;
  time(&currentTime);

  // Convert it to local time representation
  struct tm *localTime = localtime(&currentTime);
  char filename[50];
  sprintf(filename, "log/screenshots/screenshot_%d%d%d.jpg", localTime->tm_hour,
          localTime->tm_min, localTime->tm_sec);

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
      0, WINDOW_WIDTH, WINDOW_HEIGHT, 24, SDL_PIXELFORMAT_RGB24);
  if (!surface) {
    printf("Failed to create surface: %s\n", SDL_GetError());
    return;
  }

  if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGB24,
                           surface->pixels, surface->pitch) != 0) {
    printf("Failed to read pixels: %s\n", SDL_GetError());
    SDL_FreeSurface(surface);
    return;
  }

  if (IMG_SavePNG(surface, filename) != 0) {
    printf("Failed to save png: %s\n", IMG_GetError());
    SDL_FreeSurface(surface);
    return;
  }
  SDL_Delay(1000);
  SDL_FreeSurface(surface);
}


