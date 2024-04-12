#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

void drawMap(SDL_Renderer* renderer,Map* map);

void drawTransparency(SDL_Renderer* renderer,Entity player);
SDL_bool collisions(SDL_Rect* player, Map* map);
void decalageMap(Map* map,int decalage);
void decalageMur(Tuple* mur,int decalage);

#endif
