#include "homepage.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>

void renderMenuPage(SDL_Renderer *renderer) {
  TTF_Font *font = TTF_OpenFont("assets/font/VeraMono.ttf", 24);

  SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};
  // Resume Button
  SDL_Texture *resumeTexture = textTexture(renderer, font, "Resume", textColor);
  // Save Button
  SDL_Texture *saveTexture = textTexture(renderer, font, "Save", textColor);
  SDL_Texture *quitTexture = textTexture(renderer, font, "Quit", textColor);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Rect resumeRect = {(WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT - 250) / 2,
                         200, 50};
  SDL_RenderCopy(renderer, resumeTexture, NULL, &resumeRect);
  SDL_Rect saveRect = {(WINDOW_WIDTH - 200) / 2,
                       (WINDOW_HEIGHT - 250) / 2 + 100, 200, 50};
  SDL_RenderCopy(renderer, saveTexture, NULL, &saveRect);
  SDL_Rect quitRect = {(WINDOW_WIDTH - 200) / 2,
                       (WINDOW_HEIGHT - 250) / 2 + 200, 200, 50};
  SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(resumeTexture);
  SDL_DestroyTexture(saveTexture);
  SDL_DestroyTexture(quitTexture);
  TTF_CloseFont(font);
}

void renderTitlePage(SDL_Renderer *renderer, bool new) {
  TTF_Font *font = TTF_OpenFont("assets/font/VeraMono.ttf", 24);

  SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Texture *startTexture =
      textTexture(renderer, font, "New Game", textColor);
  SDL_Texture *continueTexture =
      textTexture(renderer, font, "Continue", textColor);
  SDL_Texture *quitTexture = textTexture(renderer, font, "Quit", textColor);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Rect newRect = {(WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT - 250) / 2, 200,
                      50};
  if (!new) {
    SDL_Rect continueRect = {(WINDOW_WIDTH - 200) / 2,
                             (WINDOW_HEIGHT - 250) / 2 + 100, 200, 50};
    SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);
  }
  SDL_Rect quitRect = {(WINDOW_WIDTH - 200) / 2,
                       (WINDOW_HEIGHT - 250) / 2 + 200, 200, 50};

  SDL_RenderCopy(renderer, startTexture, NULL, &newRect);
  SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(startTexture);
  if (!new) {
    SDL_DestroyTexture(continueTexture);
  }
  SDL_DestroyTexture(quitTexture);
  TTF_CloseFont(font);
}

bool launchTitlePage(SDL_Renderer *renderer, SDL_Texture **textures,
                     Game **game) {

  bool new = true;
  // check if there is a save file in the save directory
  DIR *dir = opendir(SAVE_DIRECTORY);
  if (dir == NULL) {
    perror("Failed to open directory");
    exit(-1);
  }

  struct dirent *entry;
  char lastSave[PATH_MAX];

  while ((entry = readdir(dir)) != NULL) {
    char filepath[PATH_MAX];
    snprintf(filepath, PATH_MAX, "%s%s", SAVE_DIRECTORY, entry->d_name);

    struct stat filestat;
    if (stat(filepath, &filestat) == 0 && S_ISREG(filestat.st_mode)) {
      snprintf(lastSave, PATH_MAX, "%s", filepath);
      new = false;
    }
  }
  closedir(dir);

  bool titlePage = true;
  while (titlePage) {
    renderTitlePage(renderer, new);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        titlePage = false;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
          int x = event.button.x;
          int y = event.button.y;
          SDL_Rect newRect = {(WINDOW_WIDTH - 200) / 2,
                              (WINDOW_HEIGHT - 250) / 2, 200, 50};
          SDL_Rect continueRect = {(WINDOW_WIDTH - 200) / 2,
                                   (WINDOW_HEIGHT - 250) / 2 + 100, 200, 50};
          SDL_Rect quitRect = {(WINDOW_WIDTH - 200) / 2,
                               (WINDOW_HEIGHT - 250) / 2 + 200, 200, 50};

          if (x >= newRect.x && x <= newRect.x + newRect.w && y >= newRect.y &&
              y <= newRect.y + newRect.h) {
            *game = newGame(textures, renderer);
            return true;
          }
            if (!new) {
          if (x >= continueRect.x && x <= continueRect.x + continueRect.w &&
              y >= continueRect.y && y <= continueRect.y + continueRect.h) {
            *game = loadGame(lastSave);
            return true;
          }
            }
          if (x >= quitRect.x && x <= quitRect.x + quitRect.w &&
              y >= quitRect.y && y <= quitRect.y + quitRect.h) {
            titlePage = false;
          }
        }
        break;
      }
    }
  }
  return 0;
}
