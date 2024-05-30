#ifndef __TEXT_H__
#define __TEXT_H__

#include "structs.h"
#define COUPAGE 90

#define RECT_HEIGHT 200
#define RECT_X_OFFSET 50
#define RECT_Y_OFFSET 50

void tell(Entity *player, char *message);
void renderMessage(SDL_Renderer *renderer, Entity *player);
void show_text(SDL_Renderer *renderer, char *text);
void text_display(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                  int x, int y);
char *cut_string(char *str_1);
char *difference_str(char *big_str, char *small_str);
char *copy_string(char *str);
#endif // __TEXT_H__
