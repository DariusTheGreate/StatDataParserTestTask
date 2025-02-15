#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "hashTable.h"
#include "statData.h"

int main() {
const StatData case_2_in_a[12] =
    {
    {.id = 1, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 2, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 3, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 4, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 5, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 6, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 7, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 8, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 9, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 10, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 11, .count = 1, .cost = 1, .primary = 0, .mode=3 },
    {.id = 12, .count = 1, .cost = 1, .primary = 0, .mode=3 },
   };

    StoreDump(case_2_in_a, 11, "data1.bin");
    StoreDump(case_2_in_a, 3, "data2.bin");
}


