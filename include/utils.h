#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include "defs.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);

#endif

