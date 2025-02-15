#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdio.h>

struct StatData;

typedef void (*TestFunction)(size_t);

void init_test_framework();

void register_test(const char* name, TestFunction test, size_t param);

void run_tests();

void assert_equal(StatData* expected, StatData* actual, const char* message);
void assert_not_equal(int expected, int actual, const char* message);
void assert_true(int condition, const char* message);
void assert_false(int condition, const char* message);

#endif 

