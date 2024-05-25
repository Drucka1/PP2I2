#include "game.h"
#include "player.h"

void launchGame(SDL_Renderer *renderer,TTF_Font * font)
{
  printf("Launching game...\n");
  SDL_Texture **textures = loadTextures(renderer);
  Map **rooms = loadRooms(textures);

  Map *map = rooms[0];
  printf("Map loaded\n");
  Entity *player = loadPlayer(map->spawnIndex, renderer);
  movePlayer(player, map, map->spawnIndex);
  player->moving = 0;

  SDL_Event event;
  int quit = 0;

  ///////////////////////////
  Button buttons[12];
  const char *labels = "123456789<0V";
  for (int i = 0; i < 12; ++i)
  {
    buttons[i].x = (i % 3) * (BUTTON_SIZE + BUTTON_PADDING) + BUTTON_PADDING;
    buttons[i].y = (i / 3) * (BUTTON_SIZE + BUTTON_PADDING) + 150;
    buttons[i].w = BUTTON_SIZE;
    buttons[i].h = BUTTON_SIZE;
    buttons[i].label[0] = labels[i];
    buttons[i].label[1] = '\0';
  }
  char enteredCode[15] = {0};
  int codeIndex = 0;
  char* code = "1234";

  ////////////////////////

  while (!quit)
  {
    update(player, &map, rooms);
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_MOUSEBUTTONDOWN:
        if (player->status.indigit)
        {
          int x = event.button.x;
          int y = event.button.y;
          for (int i = 0; i < 12; ++i) {
            Button b = buttons[i];
            if (x > b.x && x < b.x + b.w && y > b.y && y < b.y + b.h) {
              if(strcmp(enteredCode,"Incorrect")==0){
                codeIndex = 0;
                enteredCode[0] = '\0';
              }
              if (b.label[0] == '<') {
                if (codeIndex > 0) {
                  enteredCode[--codeIndex] = '\0';
                }
              } else if (b.label[0] == 'V') {
                if (strcmp(enteredCode, code) == 0) {
                  strcpy(enteredCode, "Correct");
                  player->status.indigit=false;
                } else {
                  strcpy(enteredCode, "Incorrect");
                }
              } else if (codeIndex < 15) {
                enteredCode[codeIndex++] = b.label[0];
                enteredCode[codeIndex] = '\0';
              }
            }
          }
        }
        break;
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_KEYUP:
        player->moving = 0;
        break;
      case SDL_KEYDOWN:
        if (player->status.icy || player->status.indigit)
        {
          break;
        }
        play(event, player, map, rooms);
        break;
      default:
        break;
      }
      update(player, &map, rooms);
      render(renderer, map, player);
      if(player->status.indigit){ //horrible mais bon pour le moment je test hein
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        for (int i = 0; i < 12; ++i) {
            drawButton(renderer, font, buttons[i]);
        }

        // Render entered code if not empty
        if (strlen(enteredCode) > 0) {
            drawText(renderer, font, enteredCode, BUTTON_PADDING, 50);
        }

        SDL_RenderPresent(renderer);
      }
    }
  }

  freeGame(player, textures, rooms);
}

void play(SDL_Event event, Entity *player, Map *map, Map **rooms)
{
  switch (event.key.keysym.sym)
  {
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
    printf("%d\n", player->status.indigit);
    printf("%d\n", player->status.icy);
    player->status.indigit=!player->status.indigit;

    break;
  default:
    break;
  }
  // moveMapBuffer(map, player);
}

void update(Entity *player, Map **map, Map **rooms)
{
  Index currentIndex = player->index;
  ListObj *current = (*map)->data[currentIndex.i][currentIndex.j]->objects;
  if (listObjContains(current, DOOR))
  {
    Object *srcDoor = listObjGet(current, DOOR);
    Path door = srcDoor->path;
    Object *destDoor = listObjGet(
        rooms[door.room]->data[door.pairedIndex.i][door.pairedIndex.j]->objects,
        DOOR);
    Index spawnIndex = srcDoor->path.pairedIndex;
    if (destDoor->facing == RIGHT)
    {
      spawnIndex.j++;
    }
    else if (destDoor->facing == UP)
    {
      spawnIndex.i--;
    }
    else if (destDoor->facing == LEFT)
    {
      spawnIndex.j--;
    }
    else if (destDoor->facing == DOWN)
    {
      spawnIndex.i++;
    }
    else
    {
    }
    if (door.open)
    {
      teleport(door.room, spawnIndex, player, map, rooms);
    }
  }
  if (listObjContains(current, ICE))
  {
    player->status.icy = true;
    Index next = nextIndex(player->index, player->facing);
    if ((*map)->data[next.i][next.j]->steppable)
    {
      movePlayer(player, *map, next);
      SDL_Delay(50);
    }
    else
    {
      player->status.icy = false;
      player->moving = 0;
    }
  }
  else
  {
    player->status.icy = false;
    player->moving = 0;
  }
  moveMapBuffer(*map, player);
}

void renderEntity(Entity *entity, SDL_Renderer *renderer)
{
  if (entity->texture == NULL)
  {
    return;
  }

  SDL_RenderCopy(renderer, entity->texture, entity->textureBuffer,
                 entity->buffer);
}

void renderPlayer(Entity *player, SDL_Renderer *renderer)
{
  if (player->texture == NULL)
  {
    return;
  }
  int h, w;
  SDL_QueryTexture(player->texture, NULL, NULL, &w, &h);
  switch (player->facing)
  {
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
  switch (player->moving)
  {
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

void renderObject(Object *object, SDL_Renderer *renderer)
{
  if (object->texture == NULL)
  {
    return;
  }
  if (object->visible == false)
  {
    return;
  }
  switch (object->objectType)
  {
  case DOOR:
    object->textureBuffer->x =
        (object->path.open) ? 3 * object->textureBuffer->w : 0;
    break;
  case LEVER:
    if (object->switchObj.state)
    {
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

void renderCell(Cell *cell, SDL_Renderer *renderer)
{
  if (cell == NULL)
  {
    return;
  }
  ListObj *current = cell->objects;
  while (current != NULL)
  {
    renderObject(current->object, renderer);
    current = current->next;
  }
}

void renderMap(Map *map, Entity *player, SDL_Renderer *renderer)
{
  double radius = (player->status.blind) ? 2.5 : 0.0;
  for (int i = renderMinI(player, map); i < renderMaxI(player, map); i++)
  {
    for (int j = renderMinJ(player, map); j < renderMaxJ(player, map); j++)
    {
      if (radius != 0 && (distance(player->index, (Index){i, j})) > radius)
      {
        continue;
      }
      renderCell(cell(i, j), renderer);
    }
  }
}

void render(SDL_Renderer *renderer, Map *map, Entity *player)
{
  moveMapBuffer(map, player);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  renderMap(map, player, renderer);
  renderPlayer(player, renderer);
  SDL_RenderPresent(renderer);
  SDL_Delay(10);
}

void freeGame(Entity *player, SDL_Texture **textures, Map **rooms)
{
  for (int i = 0; i < ROOM_COUNT; i++)
  {
    freeMap(rooms[i]);
  }
  free(rooms);
  for (int i = 0; i < TEXTURE_COUNT; i++)
  {
    SDL_DestroyTexture(textures[i]);
  }
  free(textures);
  freePlayer(player);
}

void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button button)
{
  SDL_Rect rect = {button.x, button.y, button.w, button.h};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
  SDL_RenderDrawRect(renderer, &rect);

  if (strlen(button.label) == 0)
  {
    printf("Button label is empty\n");
    return;
  }

  // Render text
  SDL_Color textColor = {0, 0, 0, 255}; // Black
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, button.label, textColor);
  if (textSurface == NULL)
  {
    printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return;
  }

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == NULL)
  {
    printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    SDL_FreeSurface(textSurface);
    return;
  }

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;
  SDL_Rect textRect = {
      button.x + (button.w - textWidth) / 2,
      button.y + (button.h - textHeight) / 2,
      textWidth,
      textHeight};

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
  if (strlen(text) == 0)
  {
    return;
  }

  SDL_Color textColor = {255, 255, 255, 255}; // White
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
  if (textSurface == NULL)
  {
    printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
    return;
  }
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == NULL)
  {
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
