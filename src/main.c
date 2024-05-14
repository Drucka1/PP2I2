#include "../include/utils.h"
#include "../include/game/rendertext.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

int main(int argc, char* args[]) {
  // Pour éviter les messages d'erreurs puisqu'on utilise pas encore les arguments de main
  (void) argc;
  (void) args;

  // Initialisation d'SDL, de la fenêtre, du moteur de rendu et de sdl_image
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  initSDL(&window, &renderer);
  

  // Lancement du jeu
  launchGame(renderer);

  //Initialisation de SDL_ttf 
  if (TTF_Init()!=0){
    fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  //Chargement de la police 
  TTF_Font *font = TTF_OpenFont("../assets/fast99.ttf",24); //taille police : 24
  if (!font){
    fprintf(stderr, "Erreur los du chargement de la police :%s\n", TTF_GetError());
    TTF_Quit();
    quitSDL(window, renderer);
    return 1;
  }

  int continu = 1;
  SDL_Event event;
  while(continu){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        continu = 0;
      }
    }

    //Effacement de l'écran 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //affichage du texte
    text_display(renderer, font, "Bienvenue dans notre escape game !", 50, 50);

    //MAJ affichage 
    SDL_RenderPresent(renderer);

  }
  TTF_CloseFont(font);
  TTF_Quit();
  // Destruction du moteur de rendu, de la fenêtre et fermeture de SDL
  quitSDL(window, renderer);
  return 0;
}



