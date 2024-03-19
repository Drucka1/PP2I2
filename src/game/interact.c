#include "../../include/game/interact.h"

void activeCell(Cell *cell)
{
  for (size_t k = 0; k < cell->numberObjects; k ++) {
    cell->objects[k]->action();
  }
}

void interact(Entity *player, Map *map)
{
  int i; int j;
  if (player->facing == FACING_RIGHT) {
    i = player->i;
    j = player->j + 1;
  } else if (player->facing == FACING_UP) {
    i = player->i - 1;
    j = player->j;
  } else if (player->facing == FACING_LEFT) {
    i = player->i;
    j = player->j - 1;
  } else if (player->facing == FACING_DOWN) {
    i = player->i + 1;
    j = player->j;
  }
  if (VALID_INDEX(i, j)) {
    activeCell(cell(i, j));
  }
}

void nothing(void) {
  printf("There is nothing to do\n");
}
