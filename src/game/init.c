#include "init.h"

void saveGame(Game *game) {
  // Get the current time
  time_t currentTime;
  time(&currentTime);

  // Convert it to local time representation
  struct tm *localTime = localtime(&currentTime);
  char filename[50];
  sprintf(filename, "log/saves/save%d%d%d.sav", localTime->tm_hour,
          localTime->tm_min, localTime->tm_sec);

  FILE *file = fopen(filename, "wb");
  if (!file) {
    printf("Failed to create file: %s\n", SDL_GetError());
    return;
  }

  fwrite(game, sizeof(Game), 1, file);
  saveRooms(game->rooms, file);
  saveMap(game->currentRoom, file);
  saveEntity(game->player, file);

  fclose(file);
}

void saveRooms(Map **rooms, FILE *file) {
  fwrite(rooms, sizeof(Map *), ROOM_COUNT, file);
  for (int i = 0; i < ROOM_COUNT; i++) {
    fwrite(rooms[i], sizeof(Map), 1, file);
  }
}

void saveMap(Map *map, FILE *file) {
  fwrite(map, sizeof(Map), 1, file);
  for (int i = 0; i < map->dimensions.i; i++) {
    for (int j = 0; j < map->dimensions.j; j++) {
      saveCell(cell(i, j), file);
    }
  }
}

void saveCell(Cell *cell, FILE *file) {
  fwrite(cell, sizeof(Cell), 1, file);
  fwrite(cell->objects, sizeof(ListObj), 1, file);
  ListObj *current = cell->objects;
  while (current) {
    saveObject(current->object, file);
    current = current->next;
  }
}

void saveListObj(ListObj *list, FILE *file) {
  fwrite(list, sizeof(ListObj), 1, file);
  ListObj *current = list;
  while (current) {
    saveObject(current->object, file);
    current = current->next;
  }
}

void saveObject(Object *object, FILE *file) {
  fwrite(object, sizeof(Object), 1, file);
  fwrite(object->buffer, sizeof(SDL_Rect), 1, file);
  if (object->textureBuffer) {
    fwrite(object->textureBuffer, sizeof(SDL_Rect), 1, file);
  }
}

void saveEntity(Entity *player, FILE *file) {
  fwrite(player, sizeof(Entity), 1, file);
  fwrite(player->buffer, sizeof(SDL_Rect), 1, file);
  fwrite(player->textureBuffer, sizeof(SDL_Rect), 1, file);

  saveListObj(player->inventory, file);
}

Game *newGame(SDL_Texture **textures, SDL_Renderer *renderer) {
  Game *game = malloc(sizeof(Game));
  game->rooms = loadRooms(textures);
  game->currentRoom = game->rooms[0];
  game->player = loadPlayer(game->currentRoom->spawnIndex, renderer);
  return game;
}

Game *loadGame(char *saveFile) {
  Game *game = malloc(sizeof(Game));
  if (!game) {
    perror("Failed to allocate memory for game");
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(saveFile, "rb");
  if (!file) {
    perror("Failed to open save file");
    free(game);
    exit(EXIT_FAILURE);
  }

  game->rooms = reloadRooms(file);
  game->currentRoom = reloadMap(file);
  game->player = reloadEntity(file);

  fclose(file);
  return game;
}

Map **reloadRooms(FILE *file) {
  Map **rooms = malloc(sizeof(Map *) * ROOM_COUNT);
  if (!rooms) {
    perror("Failed to allocate memory for rooms");
    exit(EXIT_FAILURE);
  }

  fread(rooms, sizeof(Map *), ROOM_COUNT, file);
  for (int i = 0; i < ROOM_COUNT; i++) {
    rooms[i] =  reloadMap(file);
  }

  return rooms;
}

Map *reloadMap(FILE *file) {
  Map *map = malloc(sizeof(Map));
  if (!map) {
    perror("Failed to allocate memory for map");
    exit(EXIT_FAILURE);
  }

  fread(map, sizeof(Map), 1, file);
  map->data = malloc(sizeof(Cell *) * map->dimensions.i);
  if (!map->data) {
    perror("Failed to allocate memory for map cells");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < map->dimensions.i; i++) {
    map->data[i] = malloc(sizeof(Cell) * map->dimensions.j);
    if (!map->data[i]) {
      perror("Failed to allocate memory for map cells");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < map->dimensions.j; j++) {
      map->data[i][j] = reloadCell(file);
    }
  }

  return map;
}

Cell *reloadCell(FILE *file) {
  Cell *cell = malloc(sizeof(Cell));
  fread(cell, sizeof(Cell), 1, file);
  cell->objects = reloadListObj(file);
  return cell;
}

ListObj *reloadListObj(FILE *file) {
  ListObj *list = malloc(sizeof(ListObj));
  fread(list, sizeof(ListObj), 1, file);
  list->object = reloadObject(file);
  ListObj *current = list;
  while (current->next) {
    current->next = malloc(sizeof(ListObj));
    fread(current->next, sizeof(ListObj), 1, file);
    current->next->object = reloadObject(file);
    current = current->next;
  }

  return list;
}

Object *reloadObject(FILE *file) {
  Object *object = malloc(sizeof(Object));
  fread(object, sizeof(Object), 1, file);
  object->buffer = malloc(sizeof(SDL_Rect));
  object->textureBuffer = malloc(sizeof(SDL_Rect));
  fread(object->buffer, sizeof(SDL_Rect), 1, file);
  fread(object->textureBuffer, sizeof(SDL_Rect), 1, file);
  return object;
}

Entity *reloadEntity(FILE *file) {
  Entity *player = malloc(sizeof(Entity));
  fread(player, sizeof(Entity), 1, file);
  player->buffer = malloc(sizeof(SDL_Rect));
  player->textureBuffer = malloc(sizeof(SDL_Rect));
  fread(player->buffer, sizeof(SDL_Rect), 1, file);
  fread(player->textureBuffer, sizeof(SDL_Rect), 1, file);

  player->inventory = reloadListObj(file);
  return player;
}

