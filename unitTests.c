#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "hashTable.h"
#include "statData.h"
#include "unitTests.h"

static TestFunction tests[100];
static size_t tests_params[100];
static const char* test_names[100];

volatile size_t test_count = 0;
volatile size_t failed = 0;
volatile size_t passed = 0;

void init_test_framework() {
}

void register_test(const char* name, TestFunction test, size_t param) {
    if (test_count < 100) {
        test_names[test_count] = name;
        tests[test_count] = test;
        tests_params[test_count] = param,
        test_count++;
    } else {
        printf("Test limit reached\n");
    }
}

void run_tests() {
    for(uint32_t rc = 0; rc < 1000; ++rc) { 
        printf("Iteration %d/1000:\n\n", rc+1);
        for (size_t i = 0; i < test_count; i++) {
            printf("Running %s...\n", test_names[i]);
            tests[i](tests_params[i]); // Execute the test function
        }
        printf("This iteration Tests: %ld, Passed: %ld, Failed: %ld\n\n", passed + failed, passed, failed);
    }

    printf("Total Tests: %ld, Passed: %ld, Failed: %ld\n", passed + failed, passed, failed);
}

void assert_not_equal(int expected, int actual, const char* message) {
    if (expected == actual) {
        printf("FAIL: %s - Expected not equal to %d but got %d\n", message, expected, actual);
        failed++;
    } else {
        passed++;
    }
}

void assert_true(int condition, const char* message) {
    if (!condition) {
        printf("FAIL: %s - Expected true but got false\n", message);
        failed++;
    } else {
        passed++;
    }
}

void assert_false(int condition, const char* message) {
    if (condition) {
        printf("FAIL: %s - Expected false but got true\n", message);
        failed++;
    } else {
        passed++;
    }
}

StatData* generateRandomStatData(int numElements) {
    StatData* dataArray = (StatData*)malloc(numElements * sizeof(StatData));
    if (!dataArray) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    srand(time(NULL));

    for (int i = 0; i < numElements; i++) {
        dataArray[i].id = rand() % 1000; // Random ID between 0 and 999
        dataArray[i].count = rand() % 100; // Random count between 0 and 99
        dataArray[i].cost = (float)(rand() % 10000) / 100; // Random cost between 0.00 and 99.99
        dataArray[i].primary = rand() % 2; // Random primary (0 or 1)
        dataArray[i].mode = rand() % 8; // Random mode (0 to 7, since mode is a 3-bit field)
    }

    return dataArray;
}

StatData* generateRandomStatDataWithIncreasingIds(int numElements) {
    StatData* dataArray = (StatData*)malloc(numElements * sizeof(StatData));
    if (!dataArray) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    srand(time(NULL));

    for (int i = 0; i < numElements; i++) {
        dataArray[i].id = i; 
        dataArray[i].count = rand() % 100; // Random count between 0 and 99
        dataArray[i].cost = (float)(rand() % 10000) / 100; // Random cost between 0.00 and 99.99
        dataArray[i].primary = rand() % 2; // Random primary (0 or 1)
        dataArray[i].mode = rand() % 8; // Random mode (0 to 7, since mode is a 3-bit field)
    }

    return dataArray;
}

int containsDuplicates(HashTable* table, StatData* a, StatData* b) {
    (void)a;
    (void)b;
    for(size_t j = 0; j < table->size; ++j) {
        HtItem* currentJ = table->items[j];
        while(currentJ != NULL) { 
            if(currentJ->value) {
                for (size_t i = 0; i < table->size; i++) {
                    HtItem* current = table->items[i];
                    while (current != NULL) {
                        if(current != currentJ && current->value && current->value->id == currentJ->value->id) {
                            printf("Duplicates detected %ld, %ld\n", current->value->id, currentJ->value->id);
                            a = current->value;
                            b = currentJ->value;
                            return 1;
                        }
                        current = current->next;
                    }
                }
            }
            currentJ = currentJ->next;
        }
    }

    return 0;
}

void TestHashTableInsertSearch(size_t sz)
{
    StatData* rd1 = generateRandomStatData(sz);
    HashTable* ht = create_table(sz);
    InsertDumpToHT(rd1, sz, ht);

    for(size_t i = 0; i < sz; ++i){
        char key[20];
        snprintf(key, sizeof(key), "%ld", rd1[i].id);
        StatData* found = ht_search(ht, key);
        assert_true(found != NULL, "Couldnt find key that was inserted");
    }
    
    free(rd1);
    free_table(ht);
}

void TestHashTableMergeKey(size_t p)
{
    HashTable* ht = create_table(p);
    StatData s1 = {0};
    s1.id = 1;
    char key[20];
    snprintf(key, sizeof(key), "%ld", s1.id);
    ht_merge_key(ht,key, &s1, mergeDumpItems);
    ht_merge_key(ht,key, &s1, mergeDumpItems);
    ht_merge_key(ht,key, &s1, mergeDumpItems);
    ht_merge_key(ht,key, &s1, mergeDumpItems);
    ht_merge_key(ht,key, &s1, mergeDumpItems);
    assert_true(ht->count == 1, "HT shouldnt resize on merge keys");

    StatData* a = NULL;
    StatData* b = NULL;
    assert_true(containsDuplicates(ht, a, b) == 0, "HT contains duplicates on merge");

    ht_insert(ht,key, &s1);
    ht_insert(ht,key, &s1);
    ht_insert(ht,key, &s1);
    ht_insert(ht,key, &s1);
    ht_insert(ht,key, &s1);
    assert_true(ht->count == 6, "HT can resize on insert key in case of equal keys");

    free_table(ht);
}

void TestStoreLoadOnRandomData(size_t numAttempts) {
    for(size_t i = 0; i < numAttempts; ++i) {
        StatData* rd1 = generateRandomStatData(i);
        StoreDump(rd1, i, "testRandomStatData.bin");
        size_t loadedLen = 0;
        StatData* loadedrd1 = LoadDump("testRandomStatData.bin", &loadedLen);

        assert_true(loadedLen == i, "Wrong size loaded");

        for(size_t j = 0; j < i; ++j) {
            assert_true(memcmp(loadedrd1, rd1, loadedLen) == 0, "Wrong load");
        }

        free(rd1);
        free(loadedrd1);
    }
}

void TestJoinOnRandomData(size_t sz) {
    StatData* rd1 = generateRandomStatData(sz);
    StatData* rd2 = generateRandomStatData(sz);

    size_t new_len = 0;
    StatData* joined = JoinDump(rd1, sz, rd2, sz, &new_len);

    HashTable* ht = create_table(new_len);
    InsertDumpToHT(joined, new_len, ht);

    StatData* a = NULL;
    StatData* b = NULL;
    assert_true(containsDuplicates(ht, a, b) == 0, "Duplicates detected in TestJoinOnRandomData()");

    free(rd1);
    free(rd2);
    free(joined);
    free_table(ht);
}

void TestJoinInCaseInitialArraysContainDuplicateIds(size_t sz) {
    StatData* rd1 = generateRandomStatData(sz);

    for(size_t i = 0; i < sz; i += 2) {
        rd1[i].id = 666;
    }

    StatData* rd2 = generateRandomStatData(sz * 2);

    for(size_t i = 0; i < sz*2; i += 2) {
        rd2[i].id = 666;
    }

    size_t new_len = 0; 
    StatData* joined = JoinDump(rd1, sz, rd2, sz, &new_len);

    HashTable* ht = create_table(sz * 3);
    InsertDumpToHT(joined, new_len, ht);

    StatData* a = NULL;
    StatData* b = NULL;
    assert_false(containsDuplicates(ht, a, b), "Duplicates Detected");

    free(rd1);
    free(rd2);
    free(joined);
    free_table(ht);
}

void TestAll (size_t sz) {
    StatData case_1_in_a[7] =
	{
	{.id = 1, .count = 1, .cost = 10, .primary = 0, .mode=3 },
	{.id = 2, .count = 2, .cost = 11, .primary = 1, .mode=0 },
	{.id = 3, .count = 3, .cost = 12, .primary = 1, .mode=0 },
	{.id = 4, .count = 4, .cost = 13, .primary = 1, .mode=0 },
	{.id = 5, .count = 5, .cost = 14, .primary = 1, .mode=0 },
	{.id = 6, .count = 6, .cost = 15, .primary = 1, .mode=0 },
	{.id = 7, .count = 7, .cost = 228, .primary = 1, .mode=0 },
	};

	StatData case_1_in_b[7] =
	{
	{.id = 1, .count = 1, .cost = 10, .primary = 0, .mode=3 },
	{.id = 2, .count = 2, .cost = 11, .primary = 1, .mode=0 },
	{.id = 3, .count = 3, .cost = 12, .primary = 1, .mode=0 },
	{.id = 4, .count = 4, .cost = 13, .primary = 1, .mode=0 },
	{.id = 5, .count = 5, .cost = 14, .primary = 1, .mode=0 },
	{.id = 6, .count = 6, .cost = 15, .primary = 1, .mode=0 },
	{.id = 8, .count = 8, .cost = 228, .primary = 1, .mode=0 },
	};

    StoreDump(case_1_in_a, 7, "statdata.bin");
    size_t len = 0;
    StatData* res_array = LoadDump("statdata.bin", &len);
    SortDump(res_array, len);
    PrintDump(res_array, len);
    {
        HashTable* ht = create_table(sz);

        char key1[20]; 
        snprintf(key1, sizeof(key1), "%ld", case_1_in_a[0].id);
        char key2[20]; 
        snprintf(key2, sizeof(key2), "%ld", case_1_in_a[2].id);
        
        InsertDumpToHT(res_array, len, ht);
        ht_delete(ht, key1);
        StatData* found = ht_search(ht, key1);
        if (found) {
            assert(found->id == atol(key1));
        }
        else {
            printf("Couldn't find..\n");
        }

        StatData* found2 = ht_search(ht, key2);
        if (found2) {
            assert(found2->id == atol(key2));
        }
        else {
            printf("Couldn't find..\n");
        }
        free_table(ht);
    }

    {
        size_t new_data_len = 0;
        StatData* newData = JoinDump(res_array, 7, case_1_in_b, 7, &new_data_len);
        SortDump(newData, new_data_len);
        PrintDump(newData, new_data_len);
        free(newData);
    }

    free(res_array);
}

int main() {
    init_test_framework();
    register_test("TestHashTableMergeKey().", TestHashTableMergeKey, 5);
    register_test("TestHashTableInsertSearch().", TestHashTableInsertSearch, 100000);
    register_test("TestJoinOnRandomData()", TestJoinOnRandomData, 1000000);
    register_test("TestJoinInCaseInitialArraysContainDuplicateIds().", TestJoinInCaseInitialArraysContainDuplicateIds, 100000);
    //register_test("Test StatData Store Load on random data.", TestStoreLoadOnRandomData, 10000);
    run_tests();
}

