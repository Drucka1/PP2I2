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
Object * getpush(ListObj* list){
    if(list == NULL) return NULL;
    if(list->object->type_object == PUSH) return list->object;
    return getpush(list->next);
}
void Push(Map*map,SDL_Rect*player,int direction){
    for (int i = 0;i<map->rows;i++){
        for (int j = 0;j<map->cols;j++){
            ListObj* objs = map->grid[i][j].objects;
            if(SDL_HasIntersection(player,objs->object->pos)){
                switch(direction){
                    case UP:
                    Object * push = getpush(map->grid[i-1][j].objects);
                    map->grid[i-2][j].objects = listObjAppend(map->grid[i-2][j].objects,push);
                    map->grid[i-1][j].objects = listObjRemove(map->grid[i-1][j].objects,push);
                    break;
                    case DOWN:
                    Object * push = getpush(map->grid[i+1][j].objects);
                    map->grid[i+2][j].objects = listObjAppend(map->grid[i+2][j].objects,push);
                    map->grid[i+1][j].objects = listObjRemove(map->grid[i+1][j].objects,push);
                    break;
                    case RIGHT:
                    map->grid[i][j+2].objects = listObjAppend(map->grid[i][j+2].objects,);
                    map->grid[i][j+1].objects = listObjRemove(map->grid[i][j+1].objects,push);
                    break;
                    case LEFT:
                    map->grid[i][j-2].objects = listObjAppend(map->grid[i][j-2].objects,PUSH);
                    map->grid[i][j-1].objects = listObjRemove(map->grid[i][j].objects,PUSH);
                    break;
                }
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

ListObj* listObjRemoveWall(ListObj* list) {
    if (list == NULL) return NULL;
    
    if (list->object->type_object == WALL) {
        freeListObj(list);             
        return NULL;
    } 
    list->next = listObjRemoveWall(list->next);  
    return list;
    
}

void freeListObj(ListObj *list) {
    if (list == NULL) return;
    freeObject(list->object);
    freeListObj(list->next);
    free(list);
}

void freeObject(Object *object) {
    if (object == NULL) return;
    if (object->pos != NULL) free(object->pos);
    if (object->door != NULL) free(object->door);
    free(object);
}

void freeMap(Map *map) {
    if (map == NULL) return;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            freeListObj(map->grid[i][j].objects);
            if (map->grid[i][j].map_tp) free(map->grid[i][j].map_tp);
        }
        
        free(map->grid[i]);
    }
    free(map->grid);
    free(map);
}

void freeMaps(Map** maps){
    for(int i = 0;i<NB_LEVEL;i++){
        freeMap(maps[i]);
    }
    free(maps);
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