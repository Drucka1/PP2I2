#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>

#define cell(i, j) (map->data[i][j])
#define objects(i, j) (map->data[i][j]->objects)
#define numberObjects(i, j) (map->data[i][j]->numberObjects)

#define FACING_RIGHT 0
#define FACING_UP 1
#define FACING_LEFT 2
#define FACING_DOWN 3

#define indexToPixel(index) (index * TILE_SIZE)
#define pixelToIndex(pixel) (pixel / TILE_SIZE)

typedef struct {
  char *tag;
  int i;
  int j;

  int facing;
  SDL_Rect *buffer;
  SDL_Texture *texture;
} Entity;

typedef struct {
  char *tag;
  int i;
  int j;

  int facing;
  SDL_Rect *buffer;
  SDL_Texture *texture;
} Object;

typedef struct {
  int steppable;
  Object **objects;
  size_t numberObjects;
} Cell;

typedef struct {
  Cell *data[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
} Map;

#endif 
