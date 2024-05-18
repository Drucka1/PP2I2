#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "structs.h"
#include "game/init.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);

#endif

