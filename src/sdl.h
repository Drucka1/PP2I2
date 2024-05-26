#ifndef __SDL_H__
#define __SDL_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer,TTF_Font **font);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer,TTF_Font * font);

SDL_Texture *getTexture(char *tag, SDL_Renderer *renderer);

#endif // __SDL_H__
