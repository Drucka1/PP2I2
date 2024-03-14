#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

void drawWall(SDL_Renderer* renderer,wall* mur,int nb_mur);
void drawGround(SDL_Renderer* renderer, SDL_Texture* sprite, int pos_x, int pos_y, int rows, int cols);
SDL_bool collisions(SDL_Rect* player, wall* murs, int nb_murs);
void decalage_mur(wall* murs,int nb_murs,int decalage);

#endif
