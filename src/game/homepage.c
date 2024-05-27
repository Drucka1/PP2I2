#include "homepage.h"

void renderHomepage(SDL_Renderer *renderer) {
  TTF_Font *font = TTF_OpenFont("assets/font/VeraMono.ttf", 24);

  SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Texture *startTexture =
      textTexture(renderer, font, "New Game", textColor);
  SDL_Texture *continueTexture =
      textTexture(renderer, font, "Continue", textColor);
  SDL_Texture *quitTexture = textTexture(renderer, font, "Quit", textColor);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Rect newRect = {(WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT - 150) / 2, 200,
                      50};
  SDL_Rect continueRect = {(WINDOW_WIDTH - 200) / 2,
                           (WINDOW_HEIGHT - 150) / 2 + 50, 200, 50};
  SDL_Rect quitRect = {(WINDOW_WIDTH - 200) / 2,
                       (WINDOW_HEIGHT - 150) / 2 + 100, 200, 50};

  SDL_RenderCopy(renderer, startTexture, NULL, &newRect);
  SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);
  SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(startTexture);
  SDL_DestroyTexture(quitTexture);
  TTF_CloseFont(font);
}
