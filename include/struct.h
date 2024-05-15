#ifndef STRUCT_H
#define STRUCT_H

#include "defs.h"

typedef struct {
    int x;
    int y;
} Tuple;

typedef struct {
    int level;
    int x;
    int y;
} Triple;

typedef struct {
    int type_object;
    SDL_Texture *texture;
    SDL_Rect *pos;

    //Utilisé seulement si l'objet est une clé
    Triple* door;

    // Action éventuelle réalisée par l'objet
    void (*action)(void);
} Object;

typedef struct _ListObj{
    Object* object;
    struct _ListObj* next;
} ListObj;

typedef struct {
    char* map_tp;
    bool steppable;
    ListObj* objects;
    int numberObjects;
} Cell;

typedef struct {
    int level;
    Cell** grid;
    int rows;
    int cols;
} Map;

typedef struct {
    SDL_Rect *pos;
    SDL_Texture *texture;
    ListObj* inventory;
} Entity;

ListObj* listObjAppend(ListObj* list, Object* obj);
ListObj* listObjRemove(ListObj* list, Object* obj);
ListObj* listObjRemoveWall(ListObj* list);
Object* getObject(ListObj* list, int type);
void freeListObj(ListObj* list);
void Push(Map*map,SDL_Rect*player,int direction,int positionx,int positiony);
void printlist(ListObj* list);
void printobject(Object* obj);
void freeObject(Object *object);
void freeCell(Cell *cell);
void exchangeObject(Cell *case1, Cell *case2,Tuple* nouvpos, Object* obj);
void freeMap(Map *map);
void freeMaps(Map** maps);
void freeEntity(Entity *entity);
void freeSprites(SDL_Texture** textures);

#endif