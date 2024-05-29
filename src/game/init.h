#ifndef __INIT_H__
#define __INIT_H__

#include "player.h"

void saveGame(Game *game);
void saveRooms(Map **rooms, FILE *file);
void saveMap(Map *map, FILE *file);
void saveCell(Cell *cell, FILE *file);
void saveListObj(ListObj *list, FILE *file);
void saveObject(Object *object, FILE *file);
void saveEntity(Entity *entity, FILE *file);

Game *newGame(SDL_Texture **textures, SDL_Renderer *renderer);
Game *loadGame(char *saveFile);
Map **reloadRooms(FILE *file);
Map *reloadMap(FILE *file);
Cell *reloadCell(FILE *file);
ListObj *reloadListObj(FILE *file);
Object *reloadObject(FILE *file);
Entity *reloadEntity(FILE *file);


#endif // !__INIT_H__
