#include "../src/structs.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}


void test_isIndexEqual(void) {

  Index a = {1, 2};
  Index b = {1, 2};
  TEST_ASSERT_TRUE(isIndexEqual(a, b));
}

void test_nextIndex(void) {

  Index index = {1, 2};
  Index next = nextIndex(index, RIGHT);
  TEST_ASSERT_EQUAL(1, next.i);
  TEST_ASSERT_EQUAL(3, next.j);
}

void test_distance(void) {

  Index a = {1, 2};
  Index b = {4, 6};
  double dist = distance(a, b);
  TEST_ASSERT_EQUAL_DOUBLE(5.0, dist);
}

void test_listIndexAppend(void) {

  ListIndex *list = NULL;
  Index index = {1, 2};
  listIndexAppend(&list, index, 3);
  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL(1, list->index.i);
  TEST_ASSERT_EQUAL(2, list->index.j);
  TEST_ASSERT_EQUAL(3, list->room);
  TEST_ASSERT_NULL(list->next);
}

void test_listIndexPrint(void) {

  ListIndex *list = malloc(sizeof(ListIndex));
  list->index.i = 1;
  list->index.j = 2;
  list->room = 3;
  list->next = NULL;
  listIndexPrint(list);
}

void test_listObjAppend(void) {

  ListObj *list = NULL;
  Object *obj = malloc(sizeof(Object));
  listObjAppend(&list, obj);
  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL(obj, list->object);
  TEST_ASSERT_NULL(list->next);
}

void test_listObjRemove(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj = malloc(sizeof(Object));
  obj->objectType = 2;
  obj->buffer = NULL;
  obj->textureBuffer= NULL;
  obj->texture = NULL;
  list->object = obj;
  list->next = NULL;
  listObjRemove(&list, 2);
  TEST_ASSERT_NULL(list);
  
  freeListObj(list);
}

void test_listObjContains(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj = malloc(sizeof(Object));
  obj->objectType = 2;
  list->object = obj;
  obj->buffer = NULL;
  obj->textureBuffer= NULL;
  obj->texture = NULL;
  list->next = NULL;
  bool contains = listObjContains(list, 2);
  TEST_ASSERT_TRUE(contains);
  freeListObj(list);
}

void test_listObjAdd(void) {

  ListObj *list1 = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list1->object = obj1;
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  list1->next = NULL;

  ListObj *list2 = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  list2->object = obj2;
    obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  list2->next = NULL;

  listObjAdd(&list1, list2);
  TEST_ASSERT_EQUAL(obj2, list1->next->object);
  freeListObj(list1);
}

void test_listObjPrint(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
    obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  listObjPrint(list);
  freeListObj(list);
}

void test_listObjGet(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  obj1->objectType = 1;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
    obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  Object *result = listObjGet(list, 2);
  TEST_ASSERT_EQUAL(obj2, result);
  freeListObj(list);

}

void test_listObjGetAll(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
    obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  ListObj *result = listObjGetAll(list, 2);
  TEST_ASSERT_EQUAL(obj2, result->object);
  TEST_ASSERT_NULL(result->next);
}

void test_listObjPop(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
    obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  Object *result = listObjPop(&list, 2);
  TEST_ASSERT_EQUAL(obj2, result);
  TEST_ASSERT_NULL(list->next);
  freeListObj(list);
}

void test_listObjCopy(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
    obj1->buffer = NULL;
  obj1->textureBuffer= NULL;
  obj1->texture = NULL;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  obj2->buffer = NULL;
  obj2->textureBuffer= NULL;
  obj2->texture = NULL;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  ListObj *copy = listObjCopy(list);
  TEST_ASSERT_EQUAL(obj1, copy->object);
  TEST_ASSERT_EQUAL(obj2, copy->next->object);
  TEST_ASSERT_NULL(copy->next->next);
  freeListObj(list);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_isIndexEqual);
  RUN_TEST(test_nextIndex);
  RUN_TEST(test_distance);
  RUN_TEST(test_listIndexAppend);
  RUN_TEST(test_listIndexPrint);
  RUN_TEST(test_listObjAppend);
  RUN_TEST(test_listObjRemove);
  RUN_TEST(test_listObjContains);
  RUN_TEST(test_listObjAdd);
  RUN_TEST(test_listObjPrint);
  RUN_TEST(test_listObjGet);
  RUN_TEST(test_listObjGetAll);
  RUN_TEST(test_listObjPop);
  RUN_TEST(test_listObjCopy);
  return UNITY_END();
}
