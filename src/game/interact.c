#include "interact.h"
#include "objects.h"
#include <stdio.h>
#include <strings.h>

void interact(Entity *player, Map *map, Map **rooms) {
  Index index = player->index;
  if (listObjContains(objects(player->index.i, player->index.j), KEY)) {
    printf("Found a key !\n");
    pickItem(player, map, KEY);
    return;
  } else if (listObjContains(objects(player->index.i, player->index.j),
                             LEVER)) {
    printf("Switching lever\n");
    switchLever(index, map, rooms);
    return;
  }
  if (listObjContains(objects(player->index.i, player->index.j), GENERATEUR)){
    printf("Switching electrical generator on");
    player->status.blind=false;
    return;
  }

  switch (player->facing) {
  case UP:
    index.i--;
    break;
  case DOWN:
    index.i++;
    break;
  case LEFT:
    index.j--;
    break;
  case RIGHT:
    index.j++;
    break;
  }

  if (VALID_INDEX(index)) {
    if (listObjContains(objects(index.i, index.j), DOOR)) {
      printf("opening door\n");
      openDoor(index, player, map, rooms);
      return;
    } else if (listObjContains(objects(index.i, index.j), LEVER)) {
      switchLever(index, map, rooms);
      return;
    } else if (listObjContains(objects(index.i, index.j), KEY)) {
      pickItem(player, map, KEY);
      return;
    } else if (listObjContains(objects(index.i, index.j), PUSH)) {
      pushBlock(index, player, map);
      return;
    }
  }
}

void nothing(void) { printf("There is nothing to do\n"); }
