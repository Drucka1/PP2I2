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
    if (!player->status.scary) {
      printf("Switching lever\n");
      switchLever(index, map, rooms);
      return;
    }
  }
  if (listObjContains(objects(player->index.i, player->index.j), GENERATEUR)) {
    tell(player, "Lights are back on");
    player->status.blind = false;
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
    } else if (listObjContains(objects(index.i, index.j), DIGIC)) {
      if (player->status.blind) { // on verifie si il est powered
        tell(player, "Seems like there is no power\n");
        return;
      }
      if (listObjGet(objects(index.i, index.j), DIGIC)->switchObj.state) {
        tell(player, "Code already entered");
        return;
      }
      printf("Enter code\n");
      player->status.indigit = true;
      return;
    } else if (listObjContains(objects(index.i, index.j), DOORC)) {
      printf("opening door\n");
      openDoorc(player, index, map, rooms);
      return;
    } else if (listObjContains(objects(index.i, index.j), DOOROPEN)) {
      printf("opening door\n");
      openDooropen(player, index, map, rooms);
      return;
    } else if (listObjContains(objects(index.i, index.j), LEVER)) {
      switchLever(index, map, rooms);
      return;
    } else if (listObjContains(objects(index.i, index.j), KEY)) {
      pickItem(player, map, KEY);
      return;
    } else if (listObjContains(objects(index.i, index.j), PUSH)) {
      pushBlock(index, player, map, rooms);
      return;
    }
  }
}

void nothing(void) { printf("There is nothing to do\n"); }
