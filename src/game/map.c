#include "map.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>

Object *initObject(Index index) {
  Object *object = malloc(sizeof(Object));

  object->index = index;
  object->facing = RIGHT;

  object->buffer = malloc(sizeof(SDL_Rect));
  object->buffer->x = indexToPixel(index.j);
  object->buffer->y = indexToPixel(index.i);
  object->buffer->w = TILE_SIZE;
  object->buffer->h = TILE_SIZE;

  object->textureBuffer = NULL;
  object->texture = NULL;
  object->visible = true;
  object->active = NULL;

  return object;
}

Cell *initCell(Index index) {
  Cell *cell = malloc(sizeof(Cell));
  cell->objects = malloc(sizeof(ListObj));
  cell->objects->object = initObject(index);
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
  char s[128];
  char u[128];
  int o, p, q, r;
  int h, w;
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
        Object *object = initObject((Index){i, j});
        object->objectType = GROUND;
        object->texture = textures[GROUND];
        listObjAppend(&objects, object);
      }

      else if (atoi(token) == ICE) {
        Object *object = initObject((Index){i, j});
        object->texture = textures[ICE];
        object->objectType = ICE;
        listObjAppend(&objects, object);
      }

      else if (atoi(token) == GENERATEUR) {
        Object *object = initObject((Index){i, j});
        object->texture = textures[GENERATEUR];
        object->objectType = GENERATEUR;
        listObjAppend(&objects, object);
      }

      else if (atoi(token) == WALL) {
        cell(i, j)->steppable = false;
        Object *object = initObject((Index){i, j});
        object->objectType = WALL;
        object->texture = textures[WALL];

        SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);

        object->textureBuffer = malloc(sizeof(SDL_Rect));
        object->textureBuffer->x = w / 3;
        object->textureBuffer->y = 0;
        object->textureBuffer->w = h;
        object->textureBuffer->h = h;
        listObjAppend(&objects, object);
      } else if (atoi(token) == DIGIC) {
        Object *object = initObject((Index){i, j});
        object->texture = textures[DIGIC];
        object->objectType = DIGIC;
        object->switchObj.state = false;
        SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);

        object->textureBuffer = malloc(sizeof(SDL_Rect));
        object->textureBuffer->x = w / 3;
        object->textureBuffer->y = 0;
        object->textureBuffer->w = h;
        object->textureBuffer->h = h;
        listObjAppend(&objects, object);
      } else if (atoi(token) == PUSH) {
        cell(i, j)->steppable = false;
        Object *object = initObject((Index){i, j});
        object->texture = textures[PUSH];
        object->objectType = PUSH;
        listObjAppend(&objects, object);
      }

      else if (sscanf(token, "2[%d(%d,%d)%d]", &p, &q, &r, &o) == 4) {
        Object *object = initObject((Index){i, j});
        object->texture = textures[DOOR];

        SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);
        object->textureBuffer = malloc(sizeof(SDL_Rect));
        object->textureBuffer->x = 0;
        object->textureBuffer->y = 0;
        object->textureBuffer->w = w / 4;
        object->textureBuffer->h = h;
        object->objectType = DOOR;
        object->facing = o;

        object->path.room = p;
        object->path.pairedIndex = (Index){q, r};

        cell(i, j)->steppable = false;
        object->path.open = false;

        listObjAppend(&objects, object);
      } else if (sscanf(token, "8[%d(%d,%d)%d]", &p, &q, &r, &o) ==
                 4) { // littéralement la meme chose qu'une porte classique.
        Object *object = initObject((Index){i, j});
        object->texture = textures[DOORC];

        SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);
        object->textureBuffer = malloc(sizeof(SDL_Rect));
        object->textureBuffer->x = 0;
        object->textureBuffer->y = 0;
        object->textureBuffer->w = w / 4;
        object->textureBuffer->h = h;
        object->objectType = DOORC;
        object->facing = o;

        object->path.room = p;
        object->path.pairedIndex = (Index){q, r};

        cell(i, j)->steppable = false;
        object->path.open = false;

        listObjAppend(&objects, object);
      } else if (sscanf(token, "11[%d(%d,%d)%d]", &p, &q, &r, &o) == 4) {
        Object *object = initObject((Index){i, j});
        object->texture = textures[DOOROPEN];

        SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);
        object->textureBuffer = malloc(sizeof(SDL_Rect));
        object->textureBuffer->x = 0;
        object->textureBuffer->y = 0;
        object->textureBuffer->w = w / 4;
        object->textureBuffer->h = h;
        object->objectType = DOOROPEN;
        object->facing = o;

        object->path.room = p;
        object->path.pairedIndex = (Index){q, r};

        cell(i, j)->steppable = false;
        object->path.open = false;

        listObjAppend(&objects, object);
      }

      else if (sscanf(token, "3[%s]", s) == 1) {
        Object *object = initObject((Index){i, j});
        object->switchObj.affected = NULL;
        object->texture = textures[KEY];
        object->objectType = KEY;
        object->switchObj.affected = NULL;

        char *t = strtok(s, ";");

        while (t) {
          if (sscanf(t, "%d(%d,%d)", &p, &q, &r) == 3) {
            listIndexAppend(&(object->switchObj.affected), (Index){q, r}, p);
          }
          t = strtok(NULL, ";");
        }
        listObjAppend(&objects, object);
      }

      else if (sscanf(token, "4[%s]", s) == 1) {
        Object *object = initObject((Index){i, j});
        object->objectType = LEVER;
        object->texture = textures[LEVER];
        object->switchObj.state = (atoi(u) == 1);
        object->switchObj.affected = NULL;

        char *t = strtok(s, ";");

        while (t) {
          if (sscanf(t, "%d(%d,%d)", &p, &q, &r) == 3) {
            listIndexAppend(&object->switchObj.affected, (Index){q, r}, p);
            object->switchObj.state = false;
          }

          else if (sscanf(t, "%d(%d,%d)%d", &p, &q, &r, &o) == 4) {
            if (o == 1) {
              object->switchObj.state = true;
            }
            listIndexAppend(&object->switchObj.affected, (Index){q, r}, p);
          }

          t = strtok(NULL, ";");
        }

        listObjAppend(&objects, object);
      }
    }
  }
  fclose(file);
  return map;
}

Map **loadRooms(SDL_Texture **textures) {
  Map **rooms = malloc(sizeof(Map) * ROOM_COUNT);

  if (!rooms) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate memory");
    exit(-1);
  }

  for (int i = 0; i < ROOM_COUNT; i++) {
    rooms[i] = loadMap(i, textures);
  }

  for (int k = 0; k < ROOM_COUNT; k++) {
    Map *map = rooms[k];
    for (int i = 0; i < map->dimensions.i; i++) {
      for (int j = 0; j < map->dimensions.j; j++) {
        Object *object = cell(i, j)->objects->object;
        if (object->objectType == DOOR) {
          Path door = object->path;
          door.pairedPath =
              &listObjGet(rooms[door.room]
                              ->data[door.pairedIndex.i][door.pairedIndex.j]
                              ->objects,
                          DOOR)
                   ->path;
        }
        if (object->objectType == DOORC) {
          Path door = object->path;
          door.pairedPath =
              &listObjGet(rooms[door.room]
                              ->data[door.pairedIndex.i][door.pairedIndex.j]
                              ->objects,
                          DOORC)
                   ->path;
        }
        if (object->objectType == DOOROPEN) {
          Path door = object->path;
          door.pairedPath =
              &listObjGet(rooms[door.room]
                              ->data[door.pairedIndex.i][door.pairedIndex.j]
                              ->objects,
                          DOOROPEN)
                   ->path;
        }
        if (object->objectType == LEVER) {
          if (object->switchObj.state == true) {
            switchLever((Index){i, j}, map, rooms);
          }
        }
      }
    }
  }

  return rooms;
}
