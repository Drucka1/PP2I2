#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "sdl.h"
#include <stdbool.h>

#define cell(i, j) (map->data[i][j])
#define objects(i, j) (map->data[i][j]->objects)

#define FACING_RIGHT 0
#define FACING_UP 1
#define FACING_LEFT 2
#define FACING_DOWN 3

// Conversion indice / pixel
#define indexToPixel(index) (index * TILE_SIZE)
#define pixelToIndex(pixel) (pixel / TILE_SIZE)

typedef struct {
  int i;
  int j;
} Index;

typedef struct {
  // Nom de l'objet (pour le chemin vers l'image)
  int objectType;
  // Indices de l'objet dans la carte
  Index index;

  // Orientation
  int facing;

  // Où l'image sera affiché
  SDL_Rect *buffer;
  // Image
  SDL_Texture *texture;

  // Action éventuelle réalisée par l'objet
  void (*active)(void);

  // Si l'object est une porte ou un escalier
  // Vers quelle salle il mène
  int room;
  // état de la porte
  bool open;

} Object;

typedef struct _ListObj {
  // Objet en têt
  Object *object;
  // Reste de la liste
  struct _ListObj *next;
} ListObj;

typedef struct {
  int facing;
  // Indices de l'objet dans la carte
  Index index;
  // Indices précédents
  Index prevIndex;

  // Où l'image sera affiché
  SDL_Rect *buffer;
  // Image
  SDL_Texture *texture;

  ListObj *inventory;
} Entity;

typedef struct {
  // Obstacle ou pas
  bool steppable;
  // Listes des objets contenus dans la cellule
  ListObj *objects;
} Cell;

typedef struct {
  // Niveau de la carte
  int room;
  // Carte
  Cell ***data;
  // Indices de la cllule de départ
  Index spawnIndex;
  // Dimensions de la carte
  Index dimensions;
  // Décalage de la carte
  Index offset;
} Map;

void freeObject(Object *object);
void freeListObj(ListObj *list);
void freeCell(Cell *cell);
void freeMap(Map *map);

void listObjAppend(ListObj **list, Object *obj);
void listObjRemove(ListObj **list, int objectType);
bool listObjContains(ListObj *list, int objectType);
void listObjAdd(ListObj **list, ListObj *listToAdd);

ListObj *listObjCopy(ListObj *list);

void listObjPrint(ListObj *list);

Object *listObjGet(ListObj *list, int objectType);
Object *listObjPop(ListObj **list, int objectType);

#endif // __STRUCTS_H__
