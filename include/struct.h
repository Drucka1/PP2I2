#ifndef STRUCT_H
#define STRUCT_H

#include <SDL2/SDL.h>

typedef struct _mob {
    SDL_Rect* pos;
    SDL_Texture* sprite;
} mob;

typedef struct _wall {
    SDL_Rect* pos;
    SDL_Texture* sprite;
} wall;

#endif