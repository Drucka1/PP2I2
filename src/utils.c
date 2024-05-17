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
