#include "structs.h"
#include <stdio.h>

void freeListIndex(ListIndex *list) {
  if (list == NULL) {
    return;
  }
  freeListIndex(list->next);
  free(list);
}

void freeObject(Object *object) {
  if (object->objectType == 3 || object->objectType == 4) {
    freeListIndex(object->switchObj.affected);
  }
  free(object->buffer);
  free(object);
}

void freeListObj(ListObj *list) {
  if (list == NULL)
    return;
  freeListObj(list->next);
  freeObject(list->object);
  free(list);
}

void freeCell(Cell *cell) {
  freeListObj(cell->objects);
  free(cell);
}

void freeMap(Map *map) {
  for (int i = 0; i < map->dimensions.i; i++) {
    for (int j = 0; j < map->dimensions.j; j++) {
      freeCell(cell(i, j));
    }
    free(map->data[i]);
  }
  free(map->data);
  free(map);
}

bool isIndexEqual(Index a, Index b) { return a.i == b.i && a.j == b.j; }

void listIndexAppend(ListIndex **list, Index index, int room) {
  if (*list == NULL) {
    ListIndex *new = malloc(sizeof(ListIndex));
    new->index = index;
    new->room = room;
    new->next = NULL;
    *list = new;
    return;
  }
  listIndexAppend(&((*list)->next), index, room);
}

void listIndexPrint(ListIndex *list) {
  if (list == NULL) {
    printf("\n");
    return;
  }
  printf("%d(%d, %d); ", list->room, list->index.i, list->index.j);
  listIndexPrint(list->next);
}

void listObjAppend(ListObj **list, Object *obj) {
  if (*list == NULL) {
    ListObj *new = malloc(sizeof(ListObj));
    new->object = obj;
    new->next = NULL;
    *list = new;
    return;
  }
  listObjAppend(&((*list)->next), obj);
}

void listObjRemove(ListObj **list, int objectType) {
  if (*list == NULL || (*list)->object == NULL) {
    return;
  }
  if ((*list)->object->objectType == objectType) {
    ListObj *temp = *list;
    *list = listObjCopy((*list)->next);
    freeListObj(temp);
    return;
  }
}

bool listObjContains(ListObj *list, int objectType) {
  if (list == NULL || list->object == NULL) {
    return false;
  }
  if (list->object->objectType == objectType) {
    return true;
  }
  return listObjContains(list->next, objectType);
}

void listObjAdd(ListObj **list, ListObj *listToAdd) {
  if (*list == NULL) {
    *list = listToAdd;
    return;
  }
  listObjAdd(&((*list)->next), listToAdd);
}

void listObjPrint(ListObj *list) {
  if (list == NULL || list->object == NULL) {
    printf("\n");
    return;
  }
  printf("%d; ", list->object->objectType);
  listObjPrint(list->next);
}

Object *listObjGet(ListObj *list, int objectType) {
  if (list == NULL || list->object == NULL) {
    return NULL;
  }
  if (list->object->objectType == objectType) {
    return list->object;
  }
  return listObjGet(list->next, objectType);
}

ListObj *listObjGetAll(ListObj *list, int objectType) {
  if (list == NULL || list->object == NULL) {
    return NULL;
  }
  if (list->object->objectType == objectType) {
    ListObj *new = malloc(sizeof(ListObj));
    new->object = list->object;
    new->next = listObjGetAll(list->next, objectType);
    return new;
  }
  return listObjGetAll(list->next, objectType);
}

Object *listObjPop(ListObj **list, int objectType) {
  if (*list == NULL || (*list)->object == NULL) {
    return NULL;
  }
  if ((*list)->object->objectType == objectType) {
    ListObj *temp = *list;
    Object *obj = (*list)->object;
    *list = listObjCopy((*list)->next);
    free(temp);
    return obj;
  }
  return listObjPop(&((*list)->next), objectType);
}

ListObj *listObjCopy(ListObj *list) {
  if (list == NULL) {
    return NULL;
  }
  ListObj *new = malloc(sizeof(ListObj));
  new->object = list->object;
  new->next = listObjCopy(list->next);
  return new;
}
