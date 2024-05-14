#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void text_display(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);

void display_text_letter_by_letter(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);


#endif // !RENDERTEXT_H

