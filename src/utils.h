#ifndef UTILS_H
#define UTILS_H

#include "sdl.h"

#define TEXTURE_COUNT 7

#define GROUND 0
#define WALL 1
#define DOOR 2
#define KEY 3
#define LEVER 4
#define ICE 5
#define PUSH 6

#define TEXTURE_TAGS                                                           \
  { "ground", "wall", "door", "key", "lever", "ice", "push" }

#define ROOM_COUNT 4

SDL_Texture **loadTextures(SDL_Renderer *renderer);

#endif
