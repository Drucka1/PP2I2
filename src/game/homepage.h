#ifndef __HOMEPAGE_H__
#define __HOMEPAGE_H__

#include "init.h"

void renderMenuPage(SDL_Renderer *renderer);
bool launchTitlePage(SDL_Renderer *renderer, SDL_Texture **textures,
                     Game **game);

#endif // !DEBUG
