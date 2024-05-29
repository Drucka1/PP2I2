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
    ListObj *objects =
        roomObjects(current->room, current->index.i, current->index.j);
    while (objects != NULL) {
      if (objects->object->objectType == DOOR) {
        objects->object->path.open = !objects->object->path.open;
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)
                 ->steppable;
      } else if (objects->object->objectType == WALL) {
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)
                 ->steppable;
        objects->object->visible = !objects->object->visible;
      }
      objects = objects->next;
    }
    current = current->next;
  }
}

void turnPressure(Index pressureIndex, Map *map, Map **rooms, bool on) {
  Object *pressure =
      listObjGet(objects(pressureIndex.i, pressureIndex.j), PRESSURE);
  if (pressure == NULL) {
    printf("No pressure found\n");
    exit(-1);
  }
  if (pressure->switchObj.state == on) {
    return;
  }
  pressure->switchObj.state = on;
  ListIndex *current = pressure->switchObj.affected;
  while (current != NULL) {
    ListObj *objects =
        roomObjects(current->room, current->index.i, current->index.j);
    while (objects != NULL) {
      if (objects->object->objectType == DOOR) {
        objects->object->path.open = !objects->object->path.open;
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)
                 ->steppable;
      } else if (objects->object->objectType == WALL) {
        roomCell(current->room, current->index.i, current->index.j)->steppable =
            !roomCell(current->room, current->index.i, current->index.j)
                 ->steppable;
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
  Index destIndex = door->path.pairedIndex;

  ListObj *currentObj = player->inventory;
  while (currentObj != NULL) {
    if (currentObj->object->objectType == KEY) {
    ListIndex *affected = currentObj->object->switchObj.affected;

    while (affected != NULL) {
      if (isIndexEqual(affected->index, doorIndex) ||
          isIndexEqual(affected->index, destIndex)) {
        cell(doorIndex.i, doorIndex.j)->steppable = true;
        door->path.open = true;
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
        // tell(player, "Door opened");
        return;
      }
      affected = affected->next;
    }
    }
  }


  tell(player, "This door is locked, i need to find a way to open it...");
}

void openDoorc(Index doorIndex, Map *map, Map **rooms) {
  Object *door = listObjGet(objects(doorIndex.i, doorIndex.j), DOORC);
  if (door->path.open) {
    return;
  }
  bool validate;
  if (listObjContains(objects(doorIndex.i, doorIndex.j - 1),
                      DIGIC)) { // On verifie si à gauche ou à droite il y a un
                                // digicode -> on recupere son etat
    validate = listObjGet(objects(doorIndex.i, doorIndex.j - 1), DIGIC)
                   ->switchObj.state;
  } else if (listObjContains(objects(doorIndex.i, doorIndex.j + 1), DIGIC)) {
    validate = listObjGet(objects(doorIndex.i, doorIndex.j + 1), DIGIC)
                   ->switchObj.state;
  }
  if (validate) {
    cell(doorIndex.i, doorIndex.j)->steppable = true;
    door->path.open = true;
    Index destIndex = door->path.pairedIndex;
    rooms[door->path.room]->data[destIndex.i][destIndex.j]->steppable = true;
    Object *destDoor = listObjGet(
        rooms[door->path.room]->data[destIndex.i][destIndex.j]->objects, DOORC);
    if (destDoor == NULL) {
      printf("Door is not paired\n");
      exit(-1);
    }
    destDoor->path.open = true;
    printf("Door opened\n");
    return;
  }
  printf("You need to enter the code to open this door\n");
}

void openDooropen(Index doorIndex, Map *map, Map **rooms) {
  Object *door = listObjGet(objects(doorIndex.i, doorIndex.j), DOOROPEN);
  if (door->path.open) {
    return;
  }

  cell(doorIndex.i, doorIndex.j)->steppable = true;
  door->path.open = true;
  Index destIndex = door->path.pairedIndex;
  rooms[door->path.room]->data[destIndex.i][destIndex.j]->steppable = true;

  Object *destDoor = listObjGet(
      rooms[door->path.room]->data[destIndex.i][destIndex.j]->objects,
      DOOROPEN);
  if (destDoor == NULL) {
    printf("Door is not paired\n");
    exit(-1);
  }
  destDoor->path.open = true;
  printf("Door opened\n");
  return;
  printf("You need to enter the code to open this door\n");
}

bool stackable(ListObj *objects) {
  if (objects == NULL) {
    return true;
  }
  if (objects->object->objectType == WALL ||
      objects->object->objectType == DOOR ||
      objects->object->objectType == LEVER ||
      objects->object->objectType == KEY ||
      objects->object->objectType == PUSH) {
    return false;
  }
  return stackable(objects->next);
}

bool stackablepush(ListObj *objects) {
  if (objects == NULL) {
    return true;
  }
  if (objects->object->objectType == DOOR ||
      objects->object->objectType == LEVER ||
      objects->object->objectType == KEY ||
      objects->object->objectType == PUSH) {
    return false;
  }
  return stackablepush(objects->next);
}

void fleeingLever(Entity *player, Map *map) {
  Index currentIndex = player->index;
  Index next = nextIndex(currentIndex, player->facing);
  Index next2 = nextIndex(next, player->facing);
  ListObj *adjacentObj = (map)->data[next.i][next.j]->objects;
  ListObj *current = (map)->data[currentIndex.i][currentIndex.j]->objects;
  if (listObjContains(
          current,
          LEVER)) { // levier a distance 0, il essaye de se déplacer dans le
                    // meme sens que le joueur, sinon dans le sens direct
    if (stackable(adjacentObj)) {
      moveObject(LEVER, currentIndex, map, next);
    } else if (player->facing % 2 == 0) {
      Index nextup = nextIndex(currentIndex, 1);
      ListObj *nextupObjects = (map)->data[nextup.i][nextup.j]->objects;
      if (stackable(nextupObjects)) {
        moveObject(LEVER, currentIndex, map, nextup);
      }
    }

    else {
      Index nextleft = nextIndex(currentIndex, 2);
      ListObj *nextleftObjects = (map)->data[nextleft.i][nextleft.j]->objects;
      if (stackable(nextleftObjects)) {
        moveObject(LEVER, currentIndex, map, nextleft);
      }
    }
    if (listObjContains(adjacentObj, LEVER)) {
      ListObj *nextObj =
          (map)->data[next2.i][next2.j]->objects; // levier a distance 1
      if (stackable(nextObj)) {
        moveObject(LEVER, next, map, next2);
      } else if (player->facing % 2 == 0) {
        Index nextup = nextIndex(next2, 1);
        ListObj *nextupObjects = (map)->data[nextup.i][nextup.j]->objects;
        if (stackable(nextupObjects)) {
          moveObject(LEVER, next, map, nextup);
        }
      }

      else {
        Index nextleft = nextIndex(next, 2);
        ListObj *nextleftObjects = (map)->data[nextleft.i][nextleft.j]->objects;
        if (stackable(nextleftObjects)) {
          moveObject(LEVER, next, map, nextleft);
        }
      }
    }
  }
}

void moveObject(int objectType, Index src, Map *map, Index dest) {
  if (VALID_INDEX(dest)) {
    Object *object = listObjPop(&objects(src.i, src.j), objectType);

    object->index.i = dest.i;
    object->index.j = dest.j;

    object->buffer->x = indexToPixel(object->index.j);
    object->buffer->y = indexToPixel(object->index.i);

    listObjAppend(&objects(dest.i, dest.j), object);
  }
}

void pushBlock(Index blockIndex, Entity *player, Map *map, Map **rooms) {
  Index next = nextIndex(blockIndex, player->facing);
  ListObj *nextObjects = objects(next.i, next.j);
  if (cell(next.i, next.j)->steppable == false) {
    return;
  }
  if (listObjContains(objects(blockIndex.i, blockIndex.j), PRESSURE)) {
    turnPressure(blockIndex, map, rooms, false);
  }
  if (listObjContains(nextObjects, PRESSURE)) {
    turnPressure(next, map, rooms, true);
  }
  if (stackablepush(nextObjects)) {
    cell(blockIndex.i, blockIndex.j)->steppable = true;
    cell(next.i, next.j)->steppable = false;
    moveObject(PUSH, blockIndex, map, next);
    if (listObjContains(objects(next.i, next.j), ICE)) {
      pushBlock(next, player, map, rooms);
    }
  }
}

digicode *initdigicode(char *code) {
  digicode *digi = malloc(
      sizeof(digicode)); // je crée un digicode c'est pour facilité le renderer
  const char *labels = "123456789<0V";
  for (int i = 0; i < 12; ++i) {
    digi->buttons[i].x =
        (i % 3) * (BUTTON_SIZE + BUTTON_PADDING) + BUTTON_PADDING;
    digi->buttons[i].y = (i / 3) * (BUTTON_SIZE + BUTTON_PADDING) + 150;
    digi->buttons[i].w = BUTTON_SIZE;
    digi->buttons[i].h = BUTTON_SIZE;
    digi->buttons[i].label[0] = labels[i];
    digi->buttons[i].label[1] = '\0';
  }
  digi->code = code;
  digi->enteredCode[0] = '\0'; // on stocke le code entré par le joueur
  digi->codeIndex = 0;
  return digi;
}
