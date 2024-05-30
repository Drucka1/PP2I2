#include "game.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

void launchGame(SDL_Renderer *renderer, TTF_Font *font) {
  printf("Launching game...\n");
  SDL_Texture **textures = loadTextures(renderer);
  Map **rooms = loadRooms(textures);

  Map *map = rooms[0];
  printf("Map loaded\n");
  Entity *player = loadPlayer(map->spawnIndex, renderer);
  movePlayer(player, map, map->spawnIndex);
  player->moving = 0;
  tell(player, "It's so dark in here, how could i turn on the lights ?");

  SDL_Event event;
  int quit = 0;

  ///////////////////////////
  char *code = "5272";
  digicode *digi = initdigicode(code);

  ////////////////////////

  while (!quit) {
    update(player, &map, rooms);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (player->status.home) {
          int x, y;
          SDL_GetMouseState(&x, &y);

          SDL_Rect startRect = {(WINDOW_WIDTH - 200) / 2,
                                (WINDOW_HEIGHT - 150) / 2, 200, 50};
          SDL_Rect quitRect = {(WINDOW_WIDTH - 200) / 2,
                               (WINDOW_HEIGHT - 150) / 2 + 100, 200, 50};
          // Check if start button is clicked
          if (x >= startRect.x && x <= startRect.x + startRect.w &&
              y >= startRect.y && y <= startRect.y + startRect.h) {
            player->status.home = false;
          }

          // Check if quit button is clicked
          if (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
              y >= quitRect.y && y <= quitRect.y + quitRect.h) {
            quit = true;
          }
        }
        if (player->status.indigit) {
          int x = event.button.x;
          int y = event.button.y;
          for (int i = 0; i < 12; ++i) {
            Button b = digi->buttons[i];
            if (x > b.x && x < b.x + b.w && y > b.y &&
                y < b.y + b.h) { // on check sur quel bouton on a cliqué
              if (strcmp(digi->enteredCode, "Incorrect") == 0) {
                digi->codeIndex = 0;
                digi->enteredCode[0] = '\0';
              }
              if (b.label[0] == '<') {
                if (digi->codeIndex > 0) {
                  digi->enteredCode[--digi->codeIndex] = '\0';
                }
              } else if (b.label[0] == 'V') {
                if (strcmp(digi->enteredCode, code) == 0) {
                  listObjGet(objects(player->index.i - 1, player->index.j),
                             DIGIC)
                      ->switchObj.state = true; // je me sert de l'etat activé
                  printf("Code correct\n");
                  player->status.indigit = false;
                  digi->enteredCode[0] = '\0';
                  digi->codeIndex = 0;
                } else {
                  strcpy(digi->enteredCode, "Incorrect");
                }
              } else if (digi->codeIndex < 15) {
                digi->enteredCode[digi->codeIndex++] = b.label[0];
                digi->enteredCode[digi->codeIndex] = '\0';
              }
            }
          }
        }

        break;
      case SDL_KEYUP:
        player->moving = 0;
        break;
      case SDL_KEYDOWN:
        if (player->status.icy) {
          break;
        }
        if (player->status.indigit) {
          if (event.key.keysym.sym == SDLK_ESCAPE) { // pour pouvoir faire
                                                     // escape
            player->status.indigit = false;
            digi->codeIndex = 0;
            digi->enteredCode[0] = '\0';
          }
          break;
        }
        if (player->status.speaking) {
          if (event.key.keysym.sym == SDLK_SPACE) {
            free(player->tells);
            player->status.speaking = false;
            break;
          }
          break;
        }
        play(event, player, map, rooms);
        break;
      default:
        break;
      }

      if (player->status.picture) {
        screenshot(renderer);
        player->status.picture = false;
      }
      update(player, &map, rooms);
      render(renderer, map, player, font, digi);
    }
  }
  free(digi);
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
  case SDLK_b:
    player->status.blind = !player->status.blind;
    break;
  case SDLK_m:
    player->status.indigit = !player->status.indigit;
    break;

  case SDLK_f:
    player->status.scary = !player->status.scary;
    break;
  case SDLK_ESCAPE:
    player->status.home = true;
    break;
  case SDLK_p:
    player->status.picture = true;
    break;

  default:
    break;
  }
  // moveMapBuffer(map, player);
}

void update(Entity *player, Map **map, Map **rooms) {
  Index currentIndex = player->index;
  Index next = nextIndex(player->index, player->facing);
  ListObj *current = (*map)->data[currentIndex.i][currentIndex.j]->objects;

  if (player->status.scary) {
    fleeingLever(player, *map);
  }
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
      player->facing = destDoor->facing;
      teleport(door.room, spawnIndex, player, map, rooms);
    }
  }
  if (listObjContains(current, DOORC)) {
    Object *srcDoor = listObjGet(current, DOORC);
    Path door = srcDoor->path;
    Object *destDoor = listObjGet(
        rooms[door.room]->data[door.pairedIndex.i][door.pairedIndex.j]->objects,
        DOORC);
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
  if (listObjContains(current, DOOROPEN)) {
    Object *srcDoor = listObjGet(current, DOOROPEN);
    Path door = srcDoor->path;
    Object *destDoor = listObjGet(
        rooms[door.room]->data[door.pairedIndex.i][door.pairedIndex.j]->objects,
        DOOROPEN);
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
  if (listObjContains(current, PRESSURE)) {
    bool pressure = true;
    turnPressure(currentIndex, *map, rooms, pressure);
    player->status.onpressure = true;
  }
  if (!listObjContains(current, PRESSURE) && player->status.onpressure) {
    bool pressure = false;
    turnPressure(player->prevIndex, *map, rooms, pressure);
    player->status.onpressure = false;
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
    if (object->objectType == NUMBERS) {
      SDL_SetRenderDrawColor(renderer, object->number.r, object->number.g,
                             object->number.b, 128);
      SDL_RenderFillRect(renderer, object->buffer);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
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
  case DOORC:
    object->textureBuffer->x =
        (object->path.open) ? 3 * object->textureBuffer->w : 0;
    break;
  case DOOROPEN:
    object->textureBuffer->x =
        (object->path.open) ? 3 * object->textureBuffer->w : 0;
    break;
  case LEVER:
    if (object->switchObj.state) {
      SDL_RenderCopyEx(renderer, object->texture, object->textureBuffer,
                       object->buffer, 0, NULL, SDL_FLIP_HORIZONTAL);
      return;
    }
    break;
  case NUMBERS:
    SDL_SetRenderDrawColor(renderer, object->number.r, object->number.g,
                           object->number.b, 128);
    SDL_RenderFillRect(renderer, object->buffer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    break;

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
  double radius = (player->status.blind) ? 2.5 : 0.0;
  for (int i = renderMinI(player, map); i < renderMaxI(player, map); i++) {
    for (int j = renderMinJ(player, map); j < renderMaxJ(player, map); j++) {
      if (radius != 0 && (distance(player->index, (Index){i, j})) > radius) {
        continue;
      }
      renderCell(cell(i, j), renderer);
    }
  }
}
void rendercode(SDL_Renderer *renderer, Entity *player, TTF_Font *font,
                digicode *digi) {
  if (player->status.indigit) { // horrible mais bon pour le moment je test hein
    // Définir les dimensions du rectangle
    int rectX = BUTTON_PADDING - 5;
    int rectY = 70;
    int rectW = 3 * BUTTON_SIZE + 4 * BUTTON_PADDING + 10;
    int rectH = 4 * BUTTON_SIZE + 5 * BUTTON_PADDING + 50;

    // Dessiner le rectangle gris
    SDL_Rect rect = {rectX, rectY, rectW, rectH};
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gris clair
    SDL_RenderFillRect(renderer, &rect);
    for (int i = 0; i < 12; ++i) {
      drawButton(renderer, font, digi->buttons[i]);
    }
    // Define the dimensions of the black rectangle for the code
    int codeRectX = BUTTON_PADDING;
    int codeRectY =
        rectY +
        BUTTON_PADDING; // Adjust position to be inside the grey rectangle
    int codeRectW = rectW - 2 * BUTTON_PADDING;
    int codeRectH = 40; // Height of the code display area

    // Draw the black rectangle for the code
    SDL_Rect codeRect = {codeRectX, codeRectY, codeRectW, codeRectH};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderFillRect(renderer, &codeRect);

    // Render entered code if not empty
    if (strlen(digi->enteredCode) > 0) {

      int textX = BUTTON_PADDING;
      int textY = 100;
      drawText(renderer, font, digi->enteredCode, textX, textY);
    }
  }
}

void render(SDL_Renderer *renderer, Map *map, Entity *player, TTF_Font *font,
            digicode *digi) {
  if (player->status.home) {
    renderHomepage(renderer);
    return;
  }
  moveMapBuffer(map, player);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  renderMap(map, player, renderer);
  renderPlayer(player, renderer);

  renderMessage(renderer, player);
  rendercode(renderer, player, font, digi);
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

void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button button) {
  SDL_Rect rect = {button.x, button.y, button.w, button.h};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
  SDL_RenderDrawRect(renderer, &rect);

  if (strlen(button.label) == 0) {
    printf("Button label is empty\n");
    return;
  }

  // Render text
  SDL_Color textColor = {0, 0, 0, 255}; // Black
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(font, button.label, textColor);
  if (textSurface == NULL) {
    printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return;
  }

  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    SDL_FreeSurface(textSurface);
    return;
  }

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;
  SDL_Rect textRect = {button.x + (button.w - textWidth) / 2,
                       button.y + (button.h - textHeight) / 2, textWidth,
                       textHeight};

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x,
              int y) {
  if (strlen(text) == 0) {
    return;
  }

  SDL_Color textColor = {255, 255, 255, 255}; // White
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
  if (textSurface == NULL) {
    printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return;
  }
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == NULL) {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    SDL_FreeSurface(textSurface);
    return;
  }

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;
  SDL_Rect textRect = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
}
