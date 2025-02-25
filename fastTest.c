#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "hashTable.h"
#include "statData.h"

#define FLOAT_EPSILON 1e-5 

void testCase1()
{
    const StatData case_1_in_a[2] =
    {{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};

    const StatData case_1_in_b[2] =
    {{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};

    const StatData case_1_out[3] =
    {
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2 },
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3 },
    {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2 },
    };

    assert(StoreDump(case_1_in_a, 2, "fastTestStatData.bin") == 0 && "Couldn't store fastTestStatData.bin");

    assert(StoreDump(case_1_in_b, 2, "fastTestStatData2.bin") == 0 && "Couldn't store fastTestStatData.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(ld1 && "Couldn't load generated dump at fastTestStatData.bin");
    assert(len1 == 2);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(ld2 && "Couldn't load generated dump fastTestStatData.bin");
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(joined);
    assert(len_j == 3);

    SortDump(joined, len_j);
    //PrintDump(joined, len_j);
    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_1_out[i].id);
        assert(joined[i].count == case_1_out[i].count);
        assert(fabs(joined[i].cost - case_1_out[i].cost) < FLOAT_EPSILON);
        assert(joined[i].primary == case_1_out[i].primary);
        assert(joined[i].mode == case_1_out[i].mode);
    }

    free(ld1);
    free(ld2);
    free(joined);

    printf("testCase1() passed\n");
}

void testCase2() 
{

    const StatData case_2_in_a[2] =
    {{.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 1, .cost = 1, .primary = 1, .mode=0 }};

    const StatData case_2_in_b[2] =
    {{.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=2 },
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode=2}};

    const StatData case_2_out[3] =
    {
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode = 2 },
    {.id = 90889, .count = 27, .cost = 3, .primary = 0, .mode = 3 },
    };

    assert(StoreDump(case_2_in_a, 2, "fastTestStatData.bin") == 0 && "Couldn't store fastTestStatData.bin");
    assert(StoreDump(case_2_in_b, 2, "fastTestStatData2.bin") == 0 && "Couldnt't store fastTestStatData2.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(ld1);
    assert(len1 == 2);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(ld2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(joined);
    assert(len_j == 2);
    SortDump(joined, len_j);
    //PrintDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(fabs(joined[i].cost - case_2_out[i].cost) < FLOAT_EPSILON);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 

    free(ld1);
    free(ld2);
    free(joined);
    printf("testCase2() passed\n");
}

void testCase3() 
{
    const StatData case_2_in_a[7] =
    {
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 1, .cost = 1, .primary = 1, .mode=0 }
    };

    const StatData case_2_in_b[2] =
    {{.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=2 },
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode=2}};

    const StatData case_2_out[3] =
    {
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode = 2 },
    {.id = 90889, .count = 92, .cost = 8, .primary = 0, .mode = 3 },
    };

    assert(StoreDump(case_2_in_a, 7, "fastTestStatData.bin") == 0 && "Store data fastTestStatData.bin failed");
    assert(StoreDump(case_2_in_b, 2, "fastTestStatData2.bin") == 0 && "Store data fastTestStatData.bin failed");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(ld1);
    assert(len1 == 7);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(ld2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(joined);
    assert(len_j == 2);
    SortDump(joined, len_j);
    //PrintDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(fabs(joined[i].cost - case_2_out[i].cost) < FLOAT_EPSILON);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 

    free(ld1);
    free(ld2);
    free(joined);
    printf("testCase3() passed\n");
}

// load bigie dump and compare after load
void testCase4() 
{
    const StatData case_2_in_a[86] =
    {
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=3 },
    {.id = 90889, .count = 13, .cost = 1, .primary = 1, .mode=0 },
    };

    const StatData case_2_in_b[2] =
    {{.id = 90889, .count = 13, .cost = 1, .primary = 0, .mode=2 },
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode=2}};

    const StatData case_2_out[3] =
    {
    {.id = 90189, .count = 1000, .cost = 1, .primary = 1, .mode = 2 },
    {.id = 90889, .count = 13 * 87, .cost = 87, .primary = 0, .mode = 3 },
    };

    assert(StoreDump(case_2_in_a, 86, "fastTestStatData.bin") == 0 && "Could't store fastTestStatData.bin");
    assert(StoreDump(case_2_in_b, 2, "fastTestStatData2.bin") == 0 && "Couldn't store fastTestStatData.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(ld1);
    assert(len1 == 86);

    assert(memcmp(case_2_in_a, ld1, 86) == 0);

    for(size_t i = 0; i < 86; ++i) {
        assert(ld1[i].id == case_2_in_a[i].id);
        assert(ld1[i].count == case_2_in_a[i].count);
        assert(ld1[i].cost == case_2_in_a[i].cost);
        assert(ld1[i].primary == case_2_in_a[i].primary);
        assert(ld1[i].mode == case_2_in_a[i].mode);
    }
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(ld2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(joined);
    assert(len_j == 2);
    SortDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(joined[i].cost == case_2_out[i].cost);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 

    free(ld1);
    free(ld2);
    free(joined);
    printf("testCase4() passed\n");
}



int main() {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    testCase1();
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Time taken: %f microseconds\n", cpu_time_used * 1000000.0);

    start = clock();
    testCase2();
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Time taken: %f microseconds\n", cpu_time_used * 1000000.0);

    start = clock();
    testCase3();
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Time taken: %f microseconds\n", cpu_time_used * 1000000.0);

    start = clock();
    testCase4();
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 
    printf("Time taken: %f microseconds\n", cpu_time_used * 1000000);

    printf("End of fast test\n");
    return 0;
}

