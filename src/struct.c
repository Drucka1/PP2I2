#include "../include/struct.h"

ListObj* listObjAppend(ListObj* list, Object* obj){
    if(list == NULL ){
        ListObj* next = malloc(sizeof(ListObj));
        next->object = obj;
        next->next = NULL;
        return next;
    }
    if(list->object == obj) return list;
    list->next = listObjAppend(list->next,obj);
    return list;
}
void printlist(ListObj* list){
    if(list == NULL) return;
    printf("%d\n",list->object->type_object);
    printlist(list->next);

}
void printobject(Object* obj){
    if(obj == NULL) return;
    printf("%d\n",obj->type_object);
}

Object * getObject(ListObj* list,int type){
    if(list == NULL) return NULL;
    if(list->object->type_object == type) return list->object;
    return getObject(list->next,type);
}
void Push(Map*map,SDL_Rect*player,int direction,int posx,int posy){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            if(SDL_HasIntersection(player,objs->object->pos)){
                Object * apush; //objet a pousser
                Tuple * nouvpos= malloc(sizeof(Tuple)); //nouvelle position de l'objet
                    switch(direction){
                    case UP:
                        apush = getObject(map->grid[i-1][j].objects,PUSH);
                        nouvpos->x = j*SIZE_WALL_W+posx;
                        nouvpos->y = (i-2)*SIZE_WALL_H+posy;
                        exchangeObject(&map->grid[i-1][j],&map->grid[i-2][j],nouvpos,apush);
                        break;
                    case DOWN:
                        apush = getObject(map->grid[i+1][j].objects,PUSH);
                        nouvpos->x = j*SIZE_WALL_W+posx;
                        nouvpos->y = (i+2)*SIZE_WALL_H+posy;
                        exchangeObject(&map->grid[i+1][j],&map->grid[i+2][j],nouvpos,apush);                        
                        break;
                    case RIGHT:
                        apush = getObject(map->grid[i][j+1].objects,PUSH);
                        nouvpos->x = (j+2)*SIZE_WALL_W+posx;
                        nouvpos->y = i*SIZE_WALL_H+posy;
                        exchangeObject(&map->grid[i][j+1],&map->grid[i][j+2],nouvpos,apush);
                        break;
                    case LEFT:
                        apush = getObject(map->grid[i][j-1].objects,PUSH);
                        nouvpos->x = (j-2)*SIZE_WALL_W+posx;
                        nouvpos->y = i*SIZE_WALL_H+posy;
                        exchangeObject(&map->grid[i][j-1],&map->grid[i][j-2],nouvpos,apush);
                        break;
                }
                free(nouvpos);
            }
        }
    }
}

ListObj* listObjRemove(ListObj* list, Object* obj){
    if (list == NULL) return NULL;
    if (list->object == obj){
        ListObj* next = list->next;
        free(list);
        return next;
    }
    list->next = listObjRemove(list->next,obj);
    return list;
}

ListObj* removeKeyInventory(ListObj* inventory, int lvl){
    if (!inventory) return NULL;
    if (inventory->object->door->level == lvl){
        ListObj* next = inventory->next;
        freeObject(inventory->object);
        free(inventory);
        return next;
    }
    inventory->next = removeKeyInventory(inventory->next,lvl);
    return inventory;
}

ListObj* listObjRemoveWall(ListObj* list) {
    if (list == NULL) return NULL;
    
    if (list->object->type_object == WALL) {
        freeListObj(list);             
        return NULL;
    } 
    list->next = listObjRemoveWall(list->next);  
    return list;
    
}

void exchangeObject(Cell *case1, Cell *case2,Tuple* nouvpos, Object* obj) {
    case1->objects = listObjRemove(case1->objects,obj);
    obj->pos->x = nouvpos->x;
    obj->pos->y = nouvpos->y;
    case2->numberObjects++;
    case2->objects = listObjAppend(case2->objects,obj);
}


void freeListObj(ListObj *list) {
    if (list != NULL) {
        freeObject(list->object);
        freeListObj(list->next);
        free(list);
    }
}

void freeObject(Object *object) {
    if (!object) return;
    if (object->pos) free(object->pos);
    if (object->door) free(object->door);
    free(object);
}

void freeMap(Map *map) {
    if (map == NULL) return;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            freeListObj(map->grid[i][j].objects);
            free(map->grid[i][j].map_tp);
        }
        
        free(map->grid[i]);
    }
    free(map->grid);
    free(map);
}

void freeEntity(Entity *entity) {
    if (entity == NULL) return;
    free(entity->pos);
    if (entity->inventory) freeListObj(entity->inventory);
    free(entity);
}

void freeSprites(SDL_Texture** textures){
    for(int i = 0;i<NB_SPRITES;i++){
        SDL_DestroyTexture(textures[i]);
    }
    free(textures);
}

void freePlayer(Entity* player){
    freeListObj(player->inventory);
    free(player->pos);
    SDL_DestroyTexture(player->texture);
    free(player);
}