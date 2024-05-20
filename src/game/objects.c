#include "objects.h"
#include <stdio.h>

void pickItem(Entity *player, Map *map, int objectType) {
  int i = player->index.i;
  int j = player->index.j;
  Object *item = listObjPop(&objects(i, j), objectType);
  listObjAppend(&(player->inventory), item);
}

/* void switchLever(Entity *player, Map *map) {
  int i = player->index.i;
  int j = player->index.j;
  Object *lever = listObjGet(objects(i, j), LEVER);
  lever->switchObj.state = !lever->switchObj.state;
  ListIndex *current = lever->switchObj.affected;
  while (current != NULL) {
    Cell *cell = cell(current->index.i, current->index.j);
    ListObj *objects = cell->objects;
    while (objects != NULL) {
      if (objects->object->objectType == DOOR) {
        objects->object->path.open = !objects->object->path.open;
      }
      objects = objects->next;
    }
    current = current->next;
  }
} */

void openDoor(Index doorIndex, Entity *player, Map *map, Map **rooms) {
  Object *door = listObjGet(objects(doorIndex.i, doorIndex.j), DOOR);
  if (door->path.open) {
    return;
  }
  ListObj *keys = listObjGetAll(player->inventory, KEY);
  while (keys != NULL) {
    Object *currentKey = listObjPop(&keys, KEY);
    ListIndex *affected = currentKey->switchObj.affected;
    while (affected != NULL) {
      if (isIndexEqual(affected->index, doorIndex)) {
        cell(doorIndex.i, doorIndex.j)->steppable = true;
        door->path.open = true;
        Index destIndex = door->path.pairedIndex;
        rooms[door->path.room]->data[destIndex.i][destIndex.j]->steppable =
            true;

        Object *destDoor = listObjGet(
            rooms[door->path.room]->data[destIndex.i][destIndex.j]->objects,
            DOOR);
        if (destDoor == NULL) {
          printf("Door is not paired\n");
          exit(-1);
        }
        destDoor->path.open = true;
        printf("Door opened\n");
        return;
      }
      affected = affected->next;
    }
  }
  printf("You need a key to open this door\n");
}
