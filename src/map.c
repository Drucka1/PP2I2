#include "../include/map.h"

SDL_Texture *loadTexture(char *tag, SDL_Renderer *renderer)
{
  char path[100];
  snprintf(path, sizeof(path), "assets/%s.bmp", tag);

  SDL_Surface* surface = SDL_LoadBMP(path);
  if (!surface) {
    printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
    return NULL;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

Object *loadObject(char *tag, int i, int j, int facing, SDL_Renderer *renderer)
{
  Object *object = malloc(sizeof(Object));

  object->tag = tag;
  object->i = i;
  object->j = j;
  object->facing = facing;

  object->buffer = malloc(sizeof(SDL_Rect));
  object->buffer->x = indexToPixel(j); 
  object->buffer->y = indexToPixel(i); 
  object->buffer->w = TILE_SIZE;
  object->buffer->h = TILE_SIZE;

  object->texture = loadTexture(tag, renderer);
    
  return object;
}

Cell *loadCell(int i, int j, SDL_Renderer *renderer)
{
  Cell *cell = malloc(sizeof(Cell));

  cell->steppable = 1;

  cell->numberObjects = (size_t) 1;
  cell->objects = malloc((cell->numberObjects + 1) * sizeof(Object *));
  cell->objects[cell->numberObjects - 1] = loadObject("tile128", i, j, FACING_RIGHT, renderer);
  cell->objects[cell->numberObjects] = NULL;

  return cell;
}

Map *loadMap(SDL_Renderer *renderer) 
{
  Map *map = malloc(sizeof(Map));
  
  for (int i = 0; i < MAP_MAX_HEIGHT; i++) {
    for (int j = 0; j < MAP_MAX_WIDTH; j++) {
      cell(i,j) = loadCell(i, j, renderer);
    }
  }
  return map;
}


void freeObject(Object *object)
{
  free(object->buffer);
  SDL_DestroyTexture(object->texture);
  free(object);
}

void freeCell(Cell *cell)
{
  for (size_t i = 0; i < cell->numberObjects; i++) {
    freeObject(cell->objects[i]);
  }
  free(cell->objects);
  free(cell);
}

void freeMap(Map *map)
{
  for (int i = 0; i < MAP_MAX_HEIGHT; i++) {
    for (int j = 0; j < MAP_MAX_WIDTH; j++) {
      freeCell(cell(i, j));
    }
  }
  free(map);
}
   
void renderObject(Object *object, SDL_Renderer* renderer)
{
  SDL_RenderCopy(renderer, object->texture, NULL, object->buffer);
}

void renderCell(Cell *cell, SDL_Renderer* renderer)
{
  for (size_t i = 0; i < cell->numberObjects; i++) {
    renderObject(cell->objects[i], renderer);
  }
}

void renderMap(Map *map, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < MAP_MAX_HEIGHT; i++) {
      for (int j = 0; j < MAP_MAX_WIDTH; j++) {
        renderCell(cell(i, j), renderer);
      }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}
