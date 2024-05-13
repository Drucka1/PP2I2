#include "../../include/game/rendertext.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void text_display(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y){
    
    SDL_Surface* surface_text;
    SDL_Texture* texture_text;
    SDL_Color color_text = {255,255,255}; //blanc

    surface_text = TTF_RenderText_Blended(font, text, color_text);
    if(!surface_text){
        fprint(stderr, "Erreur lors de la création de surface de texte : %s\n", TTF_GetError());
        return; 
    }

    texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
    if(!texture_text){
        fprintf(stderr, "Erreur lors de la création de texture de texte : %s\n", SDL_GetError());
        return; 
    }
    
    SDL_Rect rectangle = {x, y, surface_text->w, surface_text->h}; //w : largeur ; h : hauteur ; x et y cooronner du coin supérieur gauche du rectangle
    SDL_RenderCopy(renderer, texture_text, NULL, &rectangle);
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(texture_text);

}

