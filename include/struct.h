#ifndef STRUCT_H
#define STRUCT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Tuple;

typedef struct _List{
    int x;
    struct _List* next;
} List;

typedef struct {
    SDL_Rect *pos;
    SDL_Texture *texture;
} Entity;

typedef struct {
    int type_object;
    SDL_Texture *texture;
    SDL_Rect *pos;
    // Action éventuelle réalisée par l'objet
    void (*action)(void);
} Object;

typedef struct _ListObj{
    Object* object;
    struct _ListObj* next;
} ListObj;

typedef struct {
    bool steppable;
    ListObj* objects;
    int numberObjects;
} Cell;

typedef struct {
  Cell** grid;
  int rows;
  int cols;
} Map;

#endif