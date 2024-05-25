#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "sdl.h"
#include <math.h>
#include <stdbool.h>

#define cell(i, j) (map->data[i][j])
#define objects(i, j) (map->data[i][j]->objects)

#define roomCell(room, i, j) (rooms[room]->data[i][j])
#define roomObjects(room, i, j) (rooms[room]->data[i][j]->objects)

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

// Conversion indice / pixel
#define indexToPixel(index) (index * TILE_SIZE)
#define pixelToIndex(pixel) (pixel / TILE_SIZE)

// Vérification des indices
#define VALID_INDEX(index)                                                     \
  ((((index.i) < 0) || ((index.j) < 0) || ((index.i) >= map->dimensions.i) ||  \
    ((index.j) >= map->dimensions.j))                                          \
       ? (0)                                                                   \
       : (1))

typedef struct {
  int i;
  int j;
} Index;

typedef struct _ListIndex {
  Index index;
  int room;
  struct _ListIndex *next;
} ListIndex;

typedef struct _Path {
  // Vers quelle salle mène le passage
  int room;
  // État du passage
  bool open;
  // Indices de la cellule de destination
  Index pairedIndex;

  // Appareiller à
  struct _Path *pairedPath;

} Path; // une porte, un escalier ou un téléporteur

typedef struct {
  // État du commutateur
  bool state;
  // Objets à activer
  ListIndex *affected;

  // Durée de l'activation
  int duration;
  // Temps restant
  int timeLeft;

} Switch;

typedef struct {
  // Indices de l'objet dans la carte
  Index index;

  // Orientation
  int facing;

  // Où l'image sera affiché
  SDL_Rect *buffer;
  // Image
  SDL_Texture *texture;
  // Image de l'objet
  SDL_Rect *textureBuffer;
  // Montrer ou pas
  bool visible;
  bool fearful;

  // Action éventuelle réalisée par l'objet
  void (*active)(void);

  // Nom de l'objet (pour le chemin vers l'image)
  int objectType;
  Path path;
  Switch switchObj;

} Object;

typedef struct _ListObj {
  // Objet en têt
  Object *object;
  // Reste de la liste
  struct _ListObj *next;
} ListObj;

typedef struct {
  bool icy;
  bool blind;
  bool scary;
} Status;

typedef struct {
  int facing;
  int moving;
  // Indices de l'objet dans la carte
  Index index;
  // Indices précédents
  Index prevIndex;

  // Où l'image sera affiché
  SDL_Rect *buffer;
  // Image
  SDL_Texture *texture;
  // Image de l'objet
  SDL_Rect *textureBuffer;

  // Inventaire
  ListObj *inventory;
  // État du joueur
  Status status;
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

bool isIndexEqual(Index index1, Index index2);
Index nextIndex(Index index, int direction);
void listIndexAppend(ListIndex **list, Index index, int room);
void listIndexPrint(ListIndex *list);
double distance(Index index1, Index index2);

void listObjAppend(ListObj **list, Object *obj);
void listObjRemove(ListObj **list, int objectType);
bool listObjContains(ListObj *list, int objectType);
void listObjAdd(ListObj **list, ListObj *listToAdd);

ListObj *listObjCopy(ListObj *list);

void listObjPrint(ListObj *list);

Object *listObjGet(ListObj *list, int objectType);
ListObj *listObjGetAll(ListObj *list, int objectType);
Object *listObjPop(ListObj **list, int objectType);

#endif // __STRUCTS_H__
