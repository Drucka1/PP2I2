#include "struct.h"

List* listAppend(List* list, int x);
ListObj* listObjAppend(ListObj* list, Object* obj);
void freeList(List* list);
void freeListObj(ListObj* list);
void listPrint(List* list);