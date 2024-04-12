#include "../include/list.h"
#include "../include/struct.h"

List* listAppend(List* list, int x){
    if(list == NULL){
        List* next = malloc(sizeof(List));
        next->x = x;
        next->next = NULL;
        return next;
    }
    list->next = listAppend(list->next,x);
    return list;
}

ListObj* listObjAppend(ListObj* list, Object* obj){
    if(list == NULL){
        ListObj* next = malloc(sizeof(ListObj));
        next->object = obj;
        next->next = NULL;
        return next;
    }
    list->next = listObjAppend(list->next,obj);
    return list;
}

void freeList(List* list){
    if (list != NULL){
        freeList(list->next);
        free(list);
    }
}

void freeListObj(ListObj* list) {
    if (list != NULL) {
        freeListObj(list->next);
        free(list->object->pos);
        free(list);
    }
}

void listPrint(List* list){
    if (list != NULL) {
        printf("%d ",list->x);
        listPrint(list->next);
    }
    else {
        printf("\n");
    }
}