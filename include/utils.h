#include "defs.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);
char* getPath(bool defaultPath, int lvl);
void save_screenshot(SDL_Renderer *renderer);
