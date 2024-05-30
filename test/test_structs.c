#include "../Unity/unity.h"
#include "../src/structs.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_freeObject(void) {

  Object *object = malloc(sizeof(Object));
  object->objectType = 3;
  object->switchObj.affected = malloc(sizeof(ListIndex));
  object->switchObj.affected->next = NULL;
  freeObject(object);
  TEST_ASSERT_NULL(object);
}

void test_freeListObj(void) {

  ListObj *list = malloc(sizeof(ListObj));
  list->next = malloc(sizeof(ListObj));
  list->next->next = NULL;
  freeListObj(list);
  TEST_ASSERT_NULL(list);
}

void test_freeCell(void) {

  Cell *cell = malloc(sizeof(Cell));
  cell->objects = malloc(sizeof(ListObj));
  cell->objects->next = NULL;
  freeCell(cell);
  TEST_ASSERT_NULL(cell);
}

void test_freeMap(void) {

  Map *map = malloc(sizeof(Map));
  map->dimensions.i = 2;
  map->dimensions.j = 2;
  map->data = malloc(2 * sizeof(Cell *));
  for (int i = 0; i < 2; i++) {
    map->data[i] = malloc(2 * sizeof(Cell));
  }
  freeMap(map);
  TEST_ASSERT_NULL(map);
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
  obj->objectType = 3;
  list->object = obj;
  list->next = NULL;
  listObjRemove(&list, 3);
  TEST_ASSERT_NULL(list);
}

void test_listObjContains(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj = malloc(sizeof(Object));
  obj->objectType = 3;
  list->object = obj;
  list->next = NULL;
  bool contains = listObjContains(list, 3);
  TEST_ASSERT_TRUE(contains);
}

void test_listObjAdd(void) {

  ListObj *list1 = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list1->object = obj1;
  list1->next = NULL;

  ListObj *list2 = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  list2->object = obj2;
  list2->next = NULL;

  listObjAdd(&list1, list2);
  TEST_ASSERT_EQUAL(obj2, list1->next->object);
}

void test_listObjPrint(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  listObjPrint(list);
}

void test_listObjGet(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  Object *result = listObjGet(list, 2);
  TEST_ASSERT_EQUAL(obj2, result);
}

void test_listObjGetAll(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
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
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  Object *result = listObjPop(&list, 2);
  TEST_ASSERT_EQUAL(obj2, result);
  TEST_ASSERT_NULL(list->next);
}

void test_listObjCopy(void) {

  ListObj *list = malloc(sizeof(ListObj));
  Object *obj1 = malloc(sizeof(Object));
  obj1->objectType = 1;
  list->object = obj1;

  ListObj *next = malloc(sizeof(ListObj));
  Object *obj2 = malloc(sizeof(Object));
  obj2->objectType = 2;
  next->object = obj2;
  next->next = NULL;

  list->next = next;

  ListObj *copy = listObjCopy(list);
  TEST_ASSERT_EQUAL(obj1, copy->object);
  TEST_ASSERT_EQUAL(obj2, copy->next->object);
  TEST_ASSERT_NULL(copy->next->next);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_freeObject);
  RUN_TEST(test_freeListObj);
  RUN_TEST(test_freeCell);
  RUN_TEST(test_freeMap);
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
