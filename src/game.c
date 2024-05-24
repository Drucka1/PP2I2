#include "../include/game.h"

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
                else  if (objs->object->type_object == NUMBER) {
                    if (SDL_RenderCopy(renderer, objs->object->texture,&(SDL_Rect){.x=0,.y=0,.w=180,.h=280},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s %d", SDL_GetError(),NUMBER);
                    }
                }
                else  if (objs->object->type_object == DOOR) {
                    if (SDL_RenderCopy(renderer, objs->object->texture,&(SDL_Rect){.x=0,.y=0,.w=89,.h=94},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
                else  if (objs->object->type_object == LEVER) {
                    if (SDL_RenderCopy(renderer, objs->object->texture,&(SDL_Rect){.x=115,.y=150,.w=175,.h=200},objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
                else {
                    if (SDL_RenderCopy(renderer, objs->object->texture,NULL,objs->object->pos)){
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                    }
                }
            
                objs = objs->next;
         
            }
        }
    }
}

void drawPlayer(SDL_Renderer* renderer,Entity* player,SDL_Rect* spriteRect){
    SDL_Rect destRect = {player->pos->x, player->pos->y, SIZE_WALL_W, SIZE_WALL_H};
    if (SDL_RenderCopy(renderer, player->texture, spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
    }
}

void drawTransparency(SDL_Renderer* renderer,Entity* player){ 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
    SDL_RenderFillRect(renderer, &(SDL_Rect){player->pos->x -2*SIZE_WALL_H , player->pos->y -2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player->pos->x +2*SIZE_WALL_H , player->pos->y -2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player->pos->x -2*SIZE_WALL_H , player->pos->y +2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player->pos->x +2*SIZE_WALL_H , player->pos->y +2*SIZE_WALL_H, SIZE_WALL_W,  SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, WINDOW_WIDTH,  player->pos->y - 2*SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, player->pos->y + 3*SIZE_WALL_H, WINDOW_WIDTH,  WINDOW_HEIGHT});
    SDL_RenderFillRect(renderer, &(SDL_Rect){0, player->pos->y - 2*SIZE_WALL_H, player->pos->x - 2*SIZE_WALL_W,5*SIZE_WALL_H});
    SDL_RenderFillRect(renderer, &(SDL_Rect){player->pos->x + 3*SIZE_WALL_W, player->pos->y - 2*SIZE_WALL_H, WINDOW_WIDTH -  player->pos->x + 5*SIZE_WALL_W ,5*SIZE_WALL_H});
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
SDL_bool istype(SDL_Rect* player,Map* map,int type){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                if (objs->object->type_object == type && SDL_HasIntersection(player,objs->object->pos)){
                    return SDL_TRUE;
                }
                objs = objs->next;
            }
        }
    }
    return SDL_FALSE;
}
SDL_bool contain(ListObj* objs, int type){
    while (objs != NULL){
        if (objs->object->type_object == type){
            return SDL_TRUE;
        }
        objs = objs->next;
    }
    return SDL_FALSE;
}


SDL_bool ispushable(SDL_Rect* player,Map* map,int direction){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            while (objs != NULL){
                if (SDL_HasIntersection(player,objs->object->pos)){
                    switch(direction){
                        case UP:
                        //Si la case est steppable et qu'il n'y a pas d'autre block poussable
                        if(map->grid[i-2][j].steppable && !contain(map->grid[i-2][j].objects,PUSH)){
                            return SDL_TRUE;
                        }
                        break;
                        case DOWN:
                        if(map->grid[i+2][j].steppable && !contain(map->grid[i+2][j].objects,PUSH)){
                            return SDL_TRUE;
                        }
                        break;
                        case RIGHT:
                        if(map->grid[i][j+2].steppable && !contain(map->grid[i][j+2].objects,PUSH)){
                            return SDL_TRUE;
                        }
                        break;
                        case LEFT:
                        if(map->grid[i][j-2].steppable && !contain(map->grid[i][j-2].objects,PUSH)){
                            return SDL_TRUE;
                        }
                        break;
                    }
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

SDL_bool openDoor(ListObj* inventory,int i, int j, int level){ 
    while (inventory != NULL){
        if (inventory->object->action->x == i && inventory->object->action->y == j && level == inventory->object->action->level){
            return SDL_TRUE;
        }
        inventory = inventory->next;
    }
    return SDL_FALSE;
}

void interact(Map* *map, Entity* player,SDL_Texture** textures){
    for (int i = 0;i<(*map)->rows;i++){
        for (int j = 0;j<(*map)->cols;j++){
            
            //tp
            if ((*map)->grid[i][j].map_tp){
                SDL_Rect* pos = (*map)->grid[i][j].objects->object->pos;
                if (player->pos->x == pos->x && player->pos->y == pos->y && openDoor(player->inventory,i,j,(*map)->level)){
                    tp(map,i,j,player,textures);                        
                    return;
                }
            }

            //interation avec cle et levier
            ListObj* objs = (*map)->grid[i][j].objects;
            while (objs != NULL){
                if (objs->object->type_object == KEY && SDL_HasIntersection(player->pos,objs->object->pos)){
                    player->inventory = listObjAppend(player->inventory,objs->object);
                    (*map)->grid[i][j].objects = listObjRemove((*map)->grid[i][j].objects,objs->object);
                    return;
                }
                if (objs->object->type_object == LEVER && SDL_HasIntersection(player->pos,objs->object->pos)){
                    (*map)->grid[objs->object->action->x][objs->object->action->y].steppable = true;
                    (*map)->grid[objs->object->action->x][objs->object->action->y].objects = listObjRemoveWall((*map)->grid[objs->object->action->x][objs->object->action->y].objects);
                    return;
                }
                objs = objs->next;
            }
        }
    }
}

void savePlayerStatus(Entity* player,int current_level){
    char filename[70] = "assets/level/save/playerStatus.txt";
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }
    fprintf(file, "%d\n(%d,%d,%d,%d)\n",current_level,player->pos->x,player->pos->y,player->pos->h,player->pos->w);

    ListObj* objs = player->inventory;
    while (objs != NULL){
        fprintf(file, "%d(%d,%d,%d)\n",objs->object->type_object,objs->object->action->level,objs->object->action->x,objs->object->action->y);
        objs = objs->next;
    }

    fclose(file);
}

void tp(Map* *map,int i, int j,Entity* player,SDL_Texture** textures){
    
    if (ENABLE_SAVE){
        MapToFile(*map,(player->pos->x - (*map)->grid[0][0].objects->object->pos->x) / SIZE_WALL_W,(player->pos->y - (*map)->grid[0][0].objects->object->pos->y) / SIZE_WALL_W);
    }
    
    char* lvl = (*map)->grid[i][j].map_tp;
    char path_level[100] = "assets/level/save/";
    strcat(path_level,lvl);
    if(access(path_level, F_OK) != 0) {
        strcpy(path_level, "assets/level/default/");
        strcat(path_level,lvl);
    }

    freeMap(*map);
    int offset_player_x,offset_player_y;
    posInitPlayerLevel(path_level,&offset_player_x,&offset_player_y);
    *map = FileToMap(path_level,NULL,textures);

    player->pos->x = (*map)->offset_map.x+offset_player_x*SIZE_WALL_W;
    player->pos->y = (*map)->offset_map.y+offset_player_y*SIZE_WALL_H; 
}