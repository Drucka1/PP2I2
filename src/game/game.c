#include "game.h"
#include <stdio.h>

void launchGame(SDL_Renderer *renderer) {
  printf("Launching game...\n");
  SDL_Texture **textures = loadTextures(renderer);
  Map **rooms = loadRooms(renderer, textures);

  Map *map = rooms[0];
  printf("Map loaded\n");
  Entity *player = loadPlayer(map->spawnIndex, renderer);
  movePlayer(player, map, map->spawnIndex);

  SDL_Event event;
  int quit = 0;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      play(event, player, map);
      update(player, &map, rooms);
      render(renderer, map, player);
    }
  }

  freeGame(player, textures, rooms);
}

void play(SDL_Event event, Entity *player, Map *map) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    // Déplacement du joueur
    case SDLK_UP:
    case SDLK_z:
      moveUp(player, map);
      break;
    case SDLK_DOWN:
    case SDLK_s:
      moveDown(player, map);
      break;
    case SDLK_LEFT:
    case SDLK_q:
      moveLeft(player, map);
      break;
    case SDLK_RIGHT:
    case SDLK_d:
      moveRight(player, map);
      break;

    // Intéraction
    case SDLK_SPACE:
      interact(player, map);
      break;
    default:
      break;
    }
  }
}

void update(Entity *player, Map **map, Map **rooms) {
  Index currentIndex = player->index;
  ListObj *current = (*map)->data[currentIndex.i][currentIndex.j]->objects;
  if (listObjContains(current, DOOR)) {
    Object *door = listObjGet(current, DOOR);
    if (door->open) {
      rooms[(*map)->room]->spawnIndex = player->prevIndex;
      *map = rooms[door->room];
      movePlayer(player, *map, (*map)->spawnIndex);
    }
  }
  moveMapBuffer(*map, player);
}

void renderEntity(Entity *entity, SDL_Renderer *renderer) {
  if (entity->texture == NULL) {
    return;
  }
  SDL_RenderCopy(renderer, entity->texture, NULL, entity->buffer);
}

void renderObject(Object *object, SDL_Renderer *renderer) {
  if (object->texture == NULL) {
    return;
  }
  SDL_RenderCopy(renderer, object->texture, NULL, object->buffer);
}

void renderCell(Cell *cell, SDL_Renderer *renderer) {
  if (cell == NULL) {
    return;
  }
  ListObj *current = cell->objects;
  while (current != NULL) {
    renderObject(current->object, renderer);
    current = current->next;
  }
}

void renderMap(Map *map, Entity *player, SDL_Renderer *renderer) {
  for (int i = renderMinI(player, map); i < renderMaxI(player, map); i++) {
    for (int j = renderMinJ(player, map); j < renderMaxJ(player, map); j++) {
      renderCell(cell(i, j), renderer);
    }
  }
}

void render(SDL_Renderer *renderer, Map *map, Entity *player) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  renderMap(map, player, renderer);
  renderEntity(player, renderer);
  SDL_RenderPresent(renderer);
}

void freeGame(Entity *player, SDL_Texture **textures, Map **rooms) {
  for (int i = 0; i < ROOM_COUNT; i++) {
    freeMap(rooms[i]);
  }
  free(rooms);
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    SDL_DestroyTexture(textures[i]);
  }
  free(textures);
  freePlayer(player);
}
