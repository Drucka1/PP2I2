#include "../Unity/unity.h"

// This function is run before each test
void setUp(void) {
    // Initialization code here
}

// This function is run after each test
void tearDown(void) {
    // Clean up code here
}

// Example test case
void test_example(void) {
    TEST_ASSERT_EQUAL(1, 1); // Example assertion
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_example);
    return UNITY_END();
}
