#include "objects.h"

void pickItem(Entity *player, Map *map, int objectType) {
  int i = player->index.i;
  int j = player->index.j;
  Object *item = listObjPop(&objects(i, j), objectType);
  listObjAppend(&(player->inventory), item);
}
