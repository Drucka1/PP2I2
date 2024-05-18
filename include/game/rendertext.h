#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void text_display(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);

//void display_text_letter_by_letter(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);

char* cut_string(char* str_1);

char* difference_str(char* big_str, char* small_str);

char* copy_string(char* str);

#endif // !RENDERTEXT_H

