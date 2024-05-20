#include "interact.h"
#include <stdio.h>

void interact(Entity *player, Map *map, Map **rooms) {
  if (listObjContains(objects(player->index.i, player->index.j), KEY)) {
    printf("Found a key !\n");
    pickItem(player, map, KEY);
    return;
  }

  /* if (listObjContains(objects(player->index.i, player->index.j), LEVER)) {
    switchLever(player, map);
    return;
  } */

  Index index;
  switch (player->facing) {
  case RIGHT:
    index.i = player->index.i;
    index.j = player->index.j + 1;
    break;
  case UP:
    index.i = player->index.i - 1;
    index.j = player->index.j;
    break;
  case LEFT:
    index.i = player->index.i;
    index.j = player->index.j - 1;
    break;
  case DOWN:
    index.i = player->index.i + 1;
    index.j = player->index.j;
    break;
  }
  if (VALID_INDEX(index)) {
    if (listObjContains(objects(index.i, index.j), DOOR)) {
      openDoor(index, player, map, rooms);
      return;
    }
  }
}

void nothing(void) { printf("There is nothing to do\n"); }
