#ifndef DEFS_H
#define DEFS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "utils.h"

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 900
#define NB_LEVEL 2

#define NB_WALL_W 18
#define NB_WALL_H 18
#define SIZE_WALL_W (WINDOW_WIDTH/NB_WALL_W)
#define SIZE_WALL_H (WINDOW_HEIGHT/NB_WALL_H)
#define VITESSE SIZE_WALL_W
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

#define ENABLE_SAVE false

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define WALL 1
#define GROUND 0
#define DOOR 2
#define KEY 3
#define LEVER 4
#define ICE 5
#define PUSH 6
#define NUMBER 7

#define NB_SPRITES 8

#endif