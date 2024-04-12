#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "struct.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 900
#define NB_WALL_W 18
#define NB_WALL_H 18
#define SIZE_WALL_W (WINDOW_WIDTH/NB_WALL_W)
#define SIZE_WALL_H (WINDOW_HEIGHT/NB_WALL_H)
#define VITESSE SIZE_WALL_W
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define WALL 1
#define GROUND 0
#define DOOR 2
#define KEY 3


Map* initMap(List* **terrain,int rows,int cols, int pos_x, int pos_y, SDL_Texture** textures);
List* **FileToMap(char *nomFichier, int* rows, int* cols);
SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]);


#endif
