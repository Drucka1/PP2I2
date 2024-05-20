#include "map.h"
#include <stdio.h>

Object *initObject(Index index, int facing) {
  Object *object = malloc(sizeof(Object));

  object->index = index;
  object->facing = facing;

  object->buffer = malloc(sizeof(SDL_Rect));
  object->buffer->x = indexToPixel(index.j);
  object->buffer->y = indexToPixel(index.i);
  object->buffer->w = TILE_SIZE;
  object->buffer->h = TILE_SIZE;

  object->texture = NULL;
  object->active = NULL;

  return object;
}

Cell *initCell(Index index) {
  Cell *cell = malloc(sizeof(Cell));
  cell->objects = malloc(sizeof(ListObj));
  cell->objects->object = initObject(index, FACING_RIGHT);
  cell->objects->next = NULL;
  cell->steppable = true;
  return cell;
}

Map *loadMap(int room, SDL_Texture **textures) {
  Map *map = malloc(sizeof(Map));

  map->room = room;

  char path[100];
  snprintf(path, sizeof(path), "assets/room/%d.txt", room);

  FILE *file = fopen(path, "r");
  if (!file) {
    fprintf(stderr, "Couldn't open file: %s\n", path);
    exit(-1);
  }

  Index index;
  fscanf(file, "%d %d", &index.i, &index.j);
  map->dimensions = index;

  if (map->dimensions.i < GRID_MAX_ROW) {
    map->offset.i = (GRID_MAX_ROW - map->dimensions.i) / 2;
  } else {
    map->offset.i = 0;
  }
  if (map->dimensions.j < GRID_MAX_COLUMN) {
    map->offset.j = (GRID_MAX_COLUMN - map->dimensions.j) / 2;
  } else {
    map->offset.j = 0;
  }

  fscanf(file, "%d %d", &index.i, &index.j);
  map->spawnIndex = index;

  char token[128];
  int i = 0;
  int j = 0;
  int p, q, r;
  map->data = malloc(sizeof(Cell **) * map->dimensions.i);
  map->data[i] = malloc(sizeof(Cell *) * map->dimensions.j);
  cell(i, j) = initCell((Index){i, j});

  while (fscanf(file, "%s", token) != EOF) {
    ListObj *objects = objects(i, j);

    if (strcmp(token, "/") == 0) {
      i++;

      j = 0;
      if (i >= map->dimensions.i) {
        break;
      }
      map->data[i] = malloc(sizeof(Cell) * map->dimensions.j);
      cell(i, j) = initCell((Index){i, j});
    } else if (strcmp(token, "|") == 0) {
      j++;
      if (j >= map->dimensions.j) {
        break;
      }
      cell(i, j) = initCell((Index){i, j});
    } else {
      if (atoi(token) == GROUND) {
        Object *object = initObject((Index){i, j}, FACING_RIGHT);
        object->objectType = GROUND;
        object->texture = textures[GROUND];
        listObjAppend(&objects, object);
      } else if (atoi(token) == WALL) {
        cell(i, j)->steppable = false;
        Object *object = initObject((Index){i, j}, FACING_RIGHT);
        object->texture = textures[WALL];
        object->objectType = WALL;
        listObjAppend(&objects, object);
      } else if (sscanf(token, "2[%d(%d,%d)]", &p, &q, &r) == 3) {
        Object *object = initObject((Index){i, j}, FACING_RIGHT);
        printf("here\n");
        object->texture = textures[DOOR];
        object->objectType = DOOR;
        object->path.room = p;
        object->path.spawnIndex = (Index){q, r};

        cell(i, j)->steppable = true;
        object->path.open = true;

        listObjAppend(&objects, object);
      } else if (atoi(token) == KEY) {
        Object *object = initObject((Index){i, j}, FACING_RIGHT);
        object->texture = textures[KEY];
        object->objectType = KEY;
        listObjAppend(&objects, object);
      } else if (atoi(token) == LEVER) {
        Object *object = initObject((Index){i, j}, FACING_RIGHT);
        object->texture = textures[LEVER];
        object->objectType = LEVER;
        listObjAppend(&objects, object);
      }
    }
  }

  fclose(file);
  return map;
}

Map **loadRooms(SDL_Renderer *renderer, SDL_Texture **textures) {
  Map **rooms = malloc(sizeof(Map) * ROOM_COUNT);

  if (!rooms) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate memory");
    exit(-1);
  }

  for (int i = 0; i < ROOM_COUNT; i++) {
    rooms[i] = loadMap(i, textures);
  }

  return rooms;
}
