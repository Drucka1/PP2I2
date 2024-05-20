#include "interact.h"

void interact(Entity *player, Map *map) {
  if (listObjContains(objects(player->index.i, player->index.j), KEY)) {
    printf("Found a key !\n");
    pickItem(player, map, KEY);
    return;
  }

  Index index;
  if (player->facing == FACING_RIGHT) {
    index.i = player->index.i;
    index.j = player->index.j + 1;
  } else if (player->facing == FACING_UP) {
    index.j = player->index.j;
  } else if (player->facing == FACING_LEFT) {
    index.i = player->index.i;
    index.j = player->index.j - 1;
  } else if (player->facing == FACING_DOWN) {
    index.i = player->index.i + 1;
    index.j = player->index.j;
  }
  if (VALID_INDEX(index)) {
    // activeCell(cell(index.i, index.j));
    nothing();
  }
}

void nothing(void) { printf("There is nothing to do\n"); }

