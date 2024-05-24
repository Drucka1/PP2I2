#include "../include/utils.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    exit(-1);
  }

  (*window) = SDL_CreateWindow("Grid Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!(*window)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window: %s", SDL_GetError());
    exit(-1);
  }

  (*renderer) = SDL_CreateRenderer((*window), -1, SDL_RENDERER_ACCELERATED);
  if (!(*renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize renderer: %s", SDL_GetError());
    exit(-1);
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
    exit(-1);
  }
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

char* getPath(bool defaultPath, int lvl){
  char* buffer = malloc(sizeof(char)*50);
  if (defaultPath) sprintf(buffer, "assets/level/default/level%d.txt", lvl);
  else sprintf(buffer, "assets/level/save/level%d.txt", lvl);
  return buffer;
}

int getMaxScreenshotNum(){
    char dirpath[20] = "assets/screenshot/";
    DIR *dir = opendir(dirpath);
    if (!dir) {
        printf("Failed to open directory: %s\n", dirpath);
        return -1;
    }

    struct dirent *entry;
    int max_num = -1;
    while ((entry = readdir(dir)) != NULL) {
        int num;
        if (sscanf(entry->d_name, "screenshot_%d.jpg", &num) == 1) {
            if (num > max_num) {
                max_num = num;
            }
        }
    }

    closedir(dir);
    return max_num;
}

void save_screenshot(SDL_Renderer *renderer){
  int numScreenshot = getMaxScreenshotNum()+1;
  char filename[50];
  sprintf(filename, "assets/screenshot/screenshot_%d.jpg",numScreenshot);

  SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT, 24, SDL_PIXELFORMAT_RGB24);
  if (!surface) {
      printf("Failed to create surface: %s\n", SDL_GetError());
      return;
  }

  if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGB24, surface->pixels, surface->pitch) != 0) {
      printf("Failed to read pixels: %s\n", SDL_GetError());
      SDL_FreeSurface(surface);
      return;
  }

  if (IMG_SavePNG(surface, filename) != 0) {
      printf("Failed to save png: %s\n", IMG_GetError());
      SDL_FreeSurface(surface);
      return;
  }

  SDL_FreeSurface(surface);
}