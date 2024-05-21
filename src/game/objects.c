#include "objects.h"
#include <stdio.h>

void pickItem(Entity *player, Map *map, int objectType) {
  int i = player->index.i;
  int j = player->index.j;
  Object *item = listObjPop(&objects(i, j), objectType);
  listObjAppend(&(player->inventory), item);
}

void switchLever(Index leverIndex, Map *map, Map **rooms) {
  Object *lever = listObjGet(objects(leverIndex.i, leverIndex.j), LEVER);
  if (lever == NULL) {
    printf("No lever found\n");
    exit(-1);
  }
  lever->switchObj.state = !lever->switchObj.state;
  ListIndex *current = lever->switchObj.affected;
  while (current != NULL) {
    ListObj *objects = roomObjects(current->room, current->index.i, current->index.j);
    while (objects != NULL) {
      if (objects->object->objectType == DOOR) {
        objects->object->path.open = !objects->object->path.open;
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)->steppable;
      } else if (objects->object->objectType == WALL) {
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)->steppable;
        objects->object->visible = !objects->object->visible;
      }
      objects = objects->next;
    }
    current = current->next;
  }
}

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
