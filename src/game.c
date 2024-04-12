#include "../include/struct.h"
#include "../include/aux.h"
#include "../include/utils.h"

void drawMap(SDL_Renderer* renderer,Map* map){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++) {
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                if (objs->object->type_object == WALL){
                    if (SDL_RenderCopy(renderer, objs->object->texture, &(SDL_Rect){.x = 332 ,.y = 0, .w = 332, .h = 332},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
                else if (objs->object->type_object == GROUND){
                    if (SDL_RenderCopy(renderer, objs->object->texture,&(SDL_Rect){.x=0,.y=0,.w=256,.h=256},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
                else {
                    if (SDL_RenderCopy(renderer, objs->object->texture,&(SDL_Rect){.x=0,.y=0,.w=256,.h=256},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
                objs = objs->next;
            }
        }
    }
}

void drawTransparency(SDL_Renderer* renderer,Entity player){ 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
    SDL_RenderFillRect(renderer, &(SDL_Rect){player.pos->x -2*SIZE_WALL_H , player.pos->y -2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player.pos->x +2*SIZE_WALL_H , player.pos->y -2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player.pos->x -2*SIZE_WALL_H , player.pos->y +2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player.pos->x +2*SIZE_WALL_H , player.pos->y +2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, WINDOW_WIDTH,  player.pos->y - 2*SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, player.pos->y + 3*SIZE_WALL_H, WINDOW_WIDTH,  WINDOW_HEIGHT});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, player.pos->y - 2*SIZE_WALL_H, player.pos->x - 2*SIZE_WALL_W,5*SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player.pos->x + 3*SIZE_WALL_W, player.pos->y - 2*SIZE_WALL_H, WINDOW_WIDTH -  player.pos->x + 5*SIZE_WALL_W ,5*SIZE_WALL_H});
}

SDL_bool collisions(SDL_Rect* player, Map* map){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                if (!map->grid[i][j].steppable && SDL_HasIntersection(player,objs->object->pos)){
                    return SDL_TRUE;
                }
                objs = objs->next;
            }
        }
    }
    return SDL_FALSE;
}

void decalageMap(Map* map,int decalage){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                if (decalage == DOWN){
                    objs->object->pos->y += VITESSE;
                } else if (decalage == UP){
                    objs->object->pos->y -= VITESSE;
                } else if (decalage == RIGHT){
                    objs->object->pos->x += VITESSE;
                } else {
                    objs->object->pos->x -= VITESSE;
                }
                objs = objs->next;
            }
        }
    }  
}



void decalageMur(Tuple* mur,int decalage){
    if (decalage == DOWN){
        mur->y += VITESSE;
    } else if (decalage == UP){
        mur->y -= VITESSE;
    } else if (decalage == RIGHT){
        mur->x += VITESSE;
    } else {
        mur->x -= VITESSE;
    }
}

