#ifndef UTILS_H
#define UTILS_H

#include "sdl.h"

#define TEXTURE_COUNT 13

#define GROUND 0
#define WALL 1
#define DOOR 2
#define KEY 3
#define LEVER 4
#define ICE 5
#define PUSH 6
#define DIGIC 7
#define DOORC 8
#define FEARFULLEVER 9
#define GENERATEUR 10
#define DOOROPEN 11
#define NUMBERS 12

#define TEXTURE_TAGS                                                           \
  {                                                                            \
    "ground", "wall", "door", "key", "lever", "ice", "push", "digic", "doorc", \
        "lever", "generateur", "dooropen", "numbers"                           \
  }

#define ROOM_COUNT 9

SDL_Texture **loadTextures(SDL_Renderer *renderer);

#endif
