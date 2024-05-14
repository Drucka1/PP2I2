#include "../../include/game/rendertext.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

//Générer du texte instantanément 

void text_display(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y){
    
    SDL_Surface* surface_text;
    SDL_Texture* texture_text;
    SDL_Color color_text = {255,255,255}; //blanc

    surface_text = TTF_RenderText_Blended(font, text, color_text);
    if(!surface_text){
        fprintf(stderr, "Erreur lors de la création de surface de texte : %s\n", TTF_GetError());
        return; 
    }

    texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
    if(!texture_text){
        fprintf(stderr, "Erreur lors de la création de texture de texte : %s\n", SDL_GetError());
        return; 
    }
    
    SDL_Rect rectangle = {x, y, surface_text->w, surface_text->h}; //w : largeur ; h : hauteur ; x et y coordonnées du coin supérieur gauche du rectangle
    SDL_RenderCopy(renderer, texture_text, NULL, &rectangle);
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(texture_text);

}
/*

//Générer du texte lettre par lettre 

void display_text_letter_by_letter(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y){
    int length_text = strlen(text);
    for (int i = 0; i<length_text; i++){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255); //fond noir
        SDL_RenderClear(renderer);

        char current_text[i+2]; // lettre actuelle + '\0'
        strncpy(current_text, text, i+1);
        current_text[i+1] = '\0';
        SDL_Color color_texte = {255,255,255}; //blanc
        SDL_Surface* surface = TTF_RenderText_Solid(font, current_text, color_texte);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rectangle = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rectangle);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        SDL_RenderPresent(texture);
        SDL_Delay(100); //délai de 100ms avant d'afficher la lettre suivante
    }
}


*/
//Générer du texte lettre par lettre 
