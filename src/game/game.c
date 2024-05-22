#include "game.h"

void launchGame(SDL_Renderer *renderer) {
  printf("Launching game...\n");
  SDL_Texture **textures = loadTextures(renderer);
  Map **rooms = loadRooms(textures);

  Map *map = rooms[0];
  printf("Map loaded\n");
  Entity *player = loadPlayer(map->spawnIndex, renderer);
  movePlayer(player, map, map->spawnIndex);

  SDL_Event event;
  int quit = 0;

  while (!quit) {
    update(player, &map, rooms);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_KEYUP:
        // player->moving = 0;
        break;
      case SDL_KEYDOWN:
        if (player->status.icy) {
          break;
        }
        play(event, player, map, rooms);
        break;
      default:
        break;
      }
    }
    render(renderer, map, player);
  }

  freeGame(player, textures, rooms);
}

void play(SDL_Event event, Entity *player, Map *map, Map **rooms) {
  switch (event.key.keysym.sym) {
  // Déplacement du joueur
  case SDLK_UP:
  case SDLK_z:
    move(player, map, UP);
    break;
  case SDLK_DOWN:
  case SDLK_s:
    move(player, map, DOWN);
    break;
  case SDLK_LEFT:
  case SDLK_q:
    move(player, map, LEFT);
    break;
  case SDLK_RIGHT:
  case SDLK_d:
    move(player, map, RIGHT);
    break;

  // Intéraction
  case SDLK_SPACE:
    interact(player, map, rooms);
    break;
  default:
    break;
  }
}

void update(Entity *player, Map **map, Map **rooms) {
  Index currentIndex = player->index;
  ListObj *current = (*map)->data[currentIndex.i][currentIndex.j]->objects;
  if (listObjContains(current, DOOR)) {
    Object *srcDoor = listObjGet(current, DOOR);
    Path door = srcDoor->path;
    Object *destDoor = listObjGet(
        rooms[door.room]->data[door.pairedIndex.i][door.pairedIndex.j]->objects,
        DOOR);
    Index spawnIndex = srcDoor->path.pairedIndex;
    if (destDoor->facing == RIGHT) {
      spawnIndex.j++;
    } else if (destDoor->facing == UP) {
      spawnIndex.i--;
    } else if (destDoor->facing == LEFT) {
      spawnIndex.j--;
    } else if (destDoor->facing == DOWN) {
      spawnIndex.i++;
    } else {
    }
    if (door.open) {
      teleport(door.room, spawnIndex, player, map, rooms);
    }
  }
  if (listObjContains(current, ICE)) {
    player->status.icy = true;
    Index next = nextIndex(player->index, player->facing);
    if ((*map)->data[next.i][next.j]->steppable) {
      movePlayer(player, *map, next);
      SDL_Delay(50);
    } else {
      player->status.icy = false;
      player->moving = 0;
    }
  } else {
    player->status.icy = false;
      player->moving = 0;
  }
  moveMapBuffer(*map, player);
}

void renderEntity(Entity *entity, SDL_Renderer *renderer) {
  if (entity->texture == NULL) {
    return;
  }

  SDL_RenderCopy(renderer, entity->texture, entity->textureBuffer,
                 entity->buffer);
}

void renderPlayer(Entity *player, SDL_Renderer *renderer) {
  if (player->texture == NULL) {
    return;
  }
  int h, w;
  SDL_QueryTexture(player->texture, NULL, NULL, &w, &h);
  switch (player->facing) {
  case RIGHT:
    player->textureBuffer->y = 3 * h / 4;
    break;
  case UP:
    player->textureBuffer->y = 1 * h / 4;
    break;
  case LEFT:
    player->textureBuffer->y = 2 * h / 4;
    break;
  case DOWN:
    player->textureBuffer->y = 0 * h / 4;
    break;
  }
  switch (player->moving) {
    case 0:
      player->textureBuffer->x = 0;
      break;
    case 1:
      player->textureBuffer->x = w / 4;
      break;
    case 2:
      player->textureBuffer->x = 2 * w / 4;
      break;
    case 3:
      player->textureBuffer->x = 3 * w / 4;
      break;
  }

  SDL_RenderCopy(renderer, player->texture, player->textureBuffer,
                 player->buffer);
}

void renderObject(Object *object, SDL_Renderer *renderer) {
  if (object->texture == NULL) {
    return;
  }
  if (object->visible == false) {
    return;
  }
  switch (object->objectType) {
  case DOOR:
    object->textureBuffer->x =
        (object->path.open) ? 3 * object->textureBuffer->w : 0;
    break;
  case LEVER:
    if (object->switchObj.state) {
      SDL_RenderCopyEx(renderer, object->texture, object->textureBuffer,
                       object->buffer, 0, NULL, SDL_FLIP_HORIZONTAL);
      return;
    }
  default:
    break;
  }

  SDL_RenderCopy(renderer, object->texture, object->textureBuffer,
                 object->buffer);
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
  renderPlayer(player, renderer);
  SDL_RenderPresent(renderer);
  SDL_Delay(10);
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
