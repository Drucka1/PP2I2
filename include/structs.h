#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define cell(i, j) (map->data[i][j])
#define objects(i, j) (map->data[i][j]->objects)
#define numberObjects(i, j) (map->data[i][j]->numberObjects)

#define FACING_RIGHT 0
#define FACING_UP 1
#define FACING_LEFT 2
#define FACING_DOWN 3

// Conversion indice / pixel
#define indexToPixel(index) (index * TILE_SIZE)
#define pixelToIndex(pixel) (pixel / TILE_SIZE)

// Vérification des indices
#define VALID_INDEX(i, j) ((((i) < 0) || ((j) < 0) || ((i) >= map->height) || ((j) >= map->width)) ? (0) : (1))

typedef struct {
  char *tag;
  int i;
  int j;

  int facing;
  SDL_Rect *buffer;
  SDL_Texture *texture;
} Entity;

typedef struct {
  // Nom de l'objet (pour le chemin vers l'image)
  char *tag;
  // Indices de l'objet dans la carte
  int i; 
  int j;

  // Orientation
  int facing;

  // Où l'image sera affiché
  SDL_Rect *buffer;
  // Image 
  SDL_Texture *texture;

  // Action éventuelle réalisée par l'objet
  void (*onClick)(void);
  void (*onStep)(void);

} Object;

typedef struct {
  // Obstacle ou pas
  int steppable;
  // Listes des objets contenus dans la cellule
  Object **objects;
  // Nombre d'objets contenus dans la cellule
  size_t numberObjects;
} Cell;

typedef struct {
  Cell *data[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
  int width;
  int height;

  int i_spawn;
  int j_spawn;
} Map;

// Charge une image
SDL_Texture *loadTexture(char *tag, SDL_Renderer *renderer);

#endif 
