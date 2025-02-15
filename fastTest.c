#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "hashTable.h"
#include "statData.h"

/////////////////////////////////////////////////////////////////////

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

    StoreDump(case_1_in_a, 2, "fastTestStatData.bin");
    StoreDump(case_1_in_b, 2, "fastTestStatData2.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(len1 == 2);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);

    assert(len_j == 3);

    SortDump(joined, len_j);
    //PrintDump(joined, len_j);
    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_1_out[i].id);
        assert(joined[i].count == case_1_out[i].count);
        assert(joined[i].cost == case_1_out[i].cost);
        assert(joined[i].primary == case_1_out[i].primary);
        assert(joined[i].mode == case_1_out[i].mode);
    }
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

    StoreDump(case_2_in_a, 2, "fastTestStatData.bin");
    StoreDump(case_2_in_b, 2, "fastTestStatData2.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(len1 == 2);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(len_j == 2);
    SortDump(joined, len_j);
    //PrintDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(joined[i].cost == case_2_out[i].cost);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 
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

    StoreDump(case_2_in_a, 7, "fastTestStatData.bin");
    StoreDump(case_2_in_b, 2, "fastTestStatData2.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
    assert(len1 == 7);
    //PrintDump(ld1, len1);

    size_t len2 = 0;
    StatData* ld2 = LoadDump("fastTestStatData2.bin", &len2);
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(len_j == 2);
    SortDump(joined, len_j);
    //PrintDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(joined[i].cost == case_2_out[i].cost);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 
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

    StoreDump(case_2_in_a, 86, "fastTestStatData.bin");
    StoreDump(case_2_in_b, 2, "fastTestStatData2.bin");

    size_t len1 = 0;
    StatData* ld1 = LoadDump("fastTestStatData.bin", &len1);
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
    assert(len2 == 2);
    //PrintDump(ld2, len2);

    size_t len_j = 0;
    StatData* joined = JoinDump(ld1, len1, ld2, len2, &len_j);
    assert(len_j == 2);
    SortDump(joined, len_j);

    for(size_t i = 0; i < len_j; ++i) {
        assert(joined[i].id == case_2_out[i].id);
        assert(joined[i].count == case_2_out[i].count);
        assert(joined[i].cost == case_2_out[i].cost);
        assert(joined[i].primary == case_2_out[i].primary);
        assert(joined[i].mode == case_2_out[i].mode);
    } 
    printf("testCase4() passed\n");
}



int main() {
    testCase1();
    testCase2();
    testCase3();
    testCase4();

    printf("End of fast test\n");
    return 0;
}
