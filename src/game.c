#include "../include/struct.h"
#include "../include/aux.h"
#include "../include/init.h"

void drawWall(SDL_Renderer* renderer,wall* murs,int nb_mur){ 
    for (int i = 0;i<nb_mur;i++){
        if (SDL_RenderCopy(renderer, murs[i].sprite, &(SDL_Rect){.x = 332 ,.y = 0, .w = 332, .h = 332},murs[i].pos)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        }
    }   
}

SDL_bool collisions(SDL_Rect* player, wall* murs, int nb_murs){
    for (int i = 0;i<nb_murs;i++){
        if (SDL_HasIntersection(player,murs[i].pos)){
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void decalage_mur(wall* murs,int nb_murs,int decalage){
    for (int i = 0;i<nb_murs;i++){
        if (decalage == 0){
            murs[i].pos->y += VITESSE;
        } else if (decalage == 1){
            murs[i].pos->y -= VITESSE;
        } else if (decalage == 2){
            murs[i].pos->x += VITESSE;
        } else {
            murs[i].pos->x -= VITESSE;
        }
    }
}