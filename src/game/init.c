#include "../../include/game/init.h"
#include <SDL2/SDL_ttf.h>
#include "../../include/defs.h"
#include "../../include/game/blur.h"


void launchGame(SDL_Renderer *renderer)
{
  Map *map = loadMap(renderer);
  Entity *player = loadPlayer(0, 0, renderer);

  int quit = 0;
  SDL_Event event;
  
  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      // Intéraction entre joueur et jeu
      play(event, player, map);
    }
    
    // Affichage du jeu
    render(renderer, map, player);
    
    
  }
  // Libération de la carte et du joueur
  freeGame(player, map);
}

void render(SDL_Renderer *renderer, Map *map, Entity *player)
{
  // (Ré)initialisation de rendu 
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Affichage de la carte et du joueur
  renderMap(map, player, renderer);
  renderPlayer(player, renderer);

 
  // Afficher l'image flouttée
  SDL_Surface* surface = IMG_Load("assets/videogame.png");
  if (!surface){
      fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", "videogame.png", IMG_GetError());
      return;
  }
  
  int blurdegree = 4;
  apply_blur(surface, blurdegree);

  

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture){
    fprintf(stderr, "Unable to create texture from %s! SDL_Error: %s\n", "videogame.png", SDL_GetError());
    SDL_FreeSurface(surface);
    return ;
  }

  SDL_FreeSurface(surface);

  TTF_Font* font = TTF_OpenFont("assets/VeraMono.ttf", 24);
  if(!font){
    fprintf(stderr, "Erreur los du chargement de la police :%s\n", TTF_GetError());
    SDL_DestroyTexture(texture);
    return ;  
  }

  SDL_Color text_color = {125,255, 255, 255};
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, "PLAY", text_color);
  if (!text_surface){
    fprintf(stderr, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    return;
  }

  //Création rectangle bouton 
  SDL_Rect button_rect = {(WINDOW_WIDTH-BUTTON_WIDTH)/2, (WINDOW_HEIGHT-BUTTON_HEIGHT)/2, BUTTON_WIDTH, BUTTON_HEIGHT};
  //Création rectangle texte : 
  SDL_Rect text_rect; 
  text_rect.w = text_surface->w;
  text_rect.h = text_surface->h;
  text_rect.x = button_rect.x + (button_rect.w - text_rect.w) /2;
  text_rect.y = button_rect.y + (button_rect.h - text_rect.h) /2; 
  

  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer , text_surface);
  SDL_FreeSurface(text_surface);
  if (!text_texture){
    fprintf(stderr, "Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    return ; 
  }
  
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &button_rect);
  
  
  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);


  SDL_DestroyTexture(text_texture);
  TTF_CloseFont(font);
  SDL_DestroyTexture(texture);
  

  // Affichage dans le moteur de rendu
  SDL_RenderPresent(renderer);
  SDL_Delay(10);


}

void freeGame(Entity *player, Map *map)
{
  freePlayer(player);
  freeMap(map);
}

