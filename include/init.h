#ifndef INIT_H
#define INIT_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 900
#define NB_WALL_W 9
#define NB_WALL_H 9
#define SIZE_WALL_W (WINDOW_WIDTH/NB_WALL_W)
#define SIZE_WALL_H (WINDOW_HEIGHT/NB_WALL_H)
#define VITESSE 10

int** FileToMap(const char *nomFichier, int* rows, int* cols);
SDL_Rect* init_terrain(int** terrain,int rows,int cols);

#endif