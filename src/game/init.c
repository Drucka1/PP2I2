#include "../../include/game/init.h"
#include <SDL2/SDL_ttf.h>
#include "../../include/game/rendertext.h"
#include "../../include/defs.h"

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

  //Petit rectangle 
  SDL_Rect little_rect = {RECT_X_OFFSET, RECT_Y_OFFSET, WINDOW_WIDTH - 2* RECT_X_OFFSET, RECT_HEIGHT};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur petit rectangle 
  SDL_RenderFillRect(renderer, &little_rect);
  
  // Affichage de la carte et du joueur
  renderMap(map, player, renderer);
  renderPlayer(player, renderer);

  //Affichage texte 
  TTF_Font *font = TTF_OpenFont("assets/VeraMono.ttf",24); //taille police : 24
  if (!font){
    fprintf(stderr, "Erreur los du chargement de la police :%s\n", TTF_GetError());
    return ; //en plus 
  }

  int high_text = RECT_Y_OFFSET+10;;
  char* str = "Bienvenue dans notre escape game nous allons vous presenter les regles du jeu hdzzlejakdzejdf fzejhfze dhzejkldz azdhjzd zdjzbedez !";
  
  char* res = cut_string(str);

  text_display(renderer, font, res, RECT_X_OFFSET+10, high_text);
  char* reste = difference_str(str,res);
  char* temp_str = copy_string(reste);
  free(res);
  free(reste);
  str = temp_str;

   while (str) {
        res = cut_string(str);
        high_text+=50;
        text_display(renderer, font, res, RECT_X_OFFSET+10, high_text);
        

        reste = difference_str(str, res);
      
        if (reste == NULL) {
            free(res);
            break;
        }

        temp_str = copy_string(reste);
        free(str);
        free(res);
        free(reste);
        str = temp_str;
    }

    free(str);  

  
  
  TTF_CloseFont(font);

  // Affichage dans le moteur de rendu
  SDL_RenderPresent(renderer);
  SDL_Delay(10);
  
}

void freeGame(Entity *player, Map *map)
{
  freePlayer(player);
  freeMap(map);
}

