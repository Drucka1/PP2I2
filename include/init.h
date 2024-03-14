#ifndef INIT_H
#define INIT_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../include/struct.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 900
#define NB_WALL_W 18
#define NB_WALL_H 18
#define SIZE_WALL_W (WINDOW_WIDTH/NB_WALL_W)
#define SIZE_WALL_H (WINDOW_HEIGHT/NB_WALL_H)
#define VITESSE SIZE_WALL_W
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

int** FileToMap(char *nomFichier, int* rows, int* cols);
wall* init_terrain(int** terrain,int rows,int cols, int pos_x, int pos_y,SDL_Texture* sprite);
void free_terrain(int** terrain,int rows);
SDL_Texture* load_sprite(SDL_Renderer* renderer, char chemin[]);
void free_walls(wall* murs, int nb_murs);

#endif