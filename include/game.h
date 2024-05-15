#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "utils.h"
#include "init.h"

void drawMap(SDL_Renderer* renderer,Map* map);

void drawTransparency(SDL_Renderer* renderer,Entity* player);
void drawPlayer(SDL_Renderer* renderer,Entity* player,SDL_Rect* spriteRect);
SDL_bool collisions(SDL_Rect* player, Map* map);
void decalageMap(Map* map,int decalage);
void decalageMur(Tuple* mur,int decalage);
SDL_bool openDoor(ListObj* objs,int i, int j, int level);
void interact(Map* map, Entity* player);
SDL_bool isice(SDL_Rect* player,Map* map);
void savePlayerStatus(Entity* player,int current_level);

#endif
