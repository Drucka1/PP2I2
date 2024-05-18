#include "../../include/game/init.h"
#include <SDL2/SDL_ttf.h>
#include "../../include/game/rendertext.h"

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

  //Affichage texte 
  TTF_Font *font = TTF_OpenFont("assets/VeraMono.ttf",24); //taille police : 24
  if (!font){
    fprintf(stderr, "Erreur los du chargement de la police :%s\n", TTF_GetError());
   
  }

  int high_text = 50;
  char* str = "Bienvenue dans notre escape game nous allons vous présenter les règles du jeu !";
  
  char* res = cut_string(str);

  text_display(renderer, font, res, 130, high_text);
  char* reste = difference_str(str,res);
  char* temp_str = copy_string(reste);
  free(res);
  free(reste);
  str = temp_str;

   while (str) {
        res = cut_string(str);
        text_display(renderer, font, res, 130, high_text+50);
        high_text+=50;

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

  
  /*
  int high_text = 50;
  text_display(renderer, font, "Bienvenue dans notre escape game ! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 130, high_text); //65 caractères
  text_display(renderer, font, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 130, high_text+50); //65 caractères
  */
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

