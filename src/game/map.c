#include "../../include/game/map.h"

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

  object->action = nothing; 
    
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

Map *loadMap(char* filePath, SDL_Renderer *renderer) 
{
  FILE *file = fopen(filePath, "r");
  Map *map = malloc(sizeof(Map));

  int row, col;
  fscanf(file, "%d %d", &row, &col);
  map->height = row;
  map->width = col;

  fscanf(file, "%d %d", &row, &col);
  map->i_spawn = row;
  map->j_spawn = col;

  for (int i = 0; i < map->height; i++) {
    for (int j = 0; j < map->width; j++) {
      cell(i,j) = loadCell(i, j, renderer);

      int object;
      fscanf(file, "%d ", &object);

      if (object == 1) {
        Object *wall = loadObject("wall", i, j, FACING_RIGHT, renderer);
        wall->action = justWall;
        addObject(wall, 0, &cell(i, j));
      }
    }
  }

  fclose(file);
  return map;
}

void addObject(Object *object, int steppable, Cell **cell)
{
  (*cell)->steppable = (*cell)->steppable && steppable;
  (*cell)->numberObjects++;
  (*cell)->objects = realloc((*cell)->objects, ((*cell)->numberObjects + 1) * sizeof(Object *));
  (*cell)->objects[(*cell)->numberObjects - 1] = object;
  (*cell)->objects[(*cell)->numberObjects] = NULL;
}

void createMap(int numberRows, int numberColumns)
{
  srand(time(NULL));
  char path[100];
  sprintf(path, "assets/map/test_%d_%d.txt", numberRows, numberColumns);
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: Unable to create file %s\n", path);
    exit(0);
  }
  
  fprintf(file, "%d %d\n", numberRows, numberColumns);
  fprintf(file, "%d %d\n", numberRows/2, numberColumns/2);
  for (int i = 0; i < numberRows; i++) {
    for (int j = 0; j < numberColumns; j++) {
      if (i == numberRows/2 && j == numberColumns/2){
        fprintf(file, "%d ", 0);
      } else {
        fprintf(file, "%d ", RANDOM_INT(0, 1));
      }
    } 
    fprintf(file, "\n");
  }
  fclose(file);
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
  for (int i = 0; i < map->height; i++) {
    for (int j = 0; j < map->width; j++) {
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

void renderMap(Map *map, Entity *player, SDL_Renderer* renderer)
{
  // printf("min(i, j) = (%d, %d) ; max(i, j) = (%d, %d)\n", RENDER_MIN_I, RENDER_MIN_J, RENDER_MAX_I, RENDER_MAX_J);
  for (int i = RENDER_MIN_I; i < RENDER_MAX_I; i++) {
    for (int j = RENDER_MIN_J; j < RENDER_MAX_J; j++) {
      renderCell(cell(i, j), renderer);
    }
  }
}

