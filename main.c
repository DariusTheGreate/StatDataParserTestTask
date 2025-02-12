#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "statData.h"

int main(){
	printf("hello there\n");
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

    printf("\n Store() Load() Test\n");
    // Fast Test store/load/print.
    StoreDump(case_1_in_a, 7, "statdata.bin");

    size_t len = 0;
    StatData* res_array = LoadDump("statdata.bin", &len);

    SortDump(res_array, len);
    PrintDump(res_array, len);

    printf("\nHashTable Test\n");
    {
        HashTable* ht = create_table(50);

        char key1[20]; 
        snprintf(key1, sizeof(key1), "%ld", case_1_in_a[0].id);
        char key2[20]; 
        snprintf(key2, sizeof(key2), "%ld", case_1_in_a[2].id);
        
        InsertDumpToHT(res_array, len, ht);
        printf("Inserted dump to ht\n");
        printf("Try to delete key1=%ld\n", atol(key1));
        ht_delete(ht, key1);
        printf("Search for key1\n");
        StatData* found = ht_search(ht, key1);
        if (found) {
            assert(found->id == atol(key1));
            printf("Found data with id %s: count=%d\n", key1, found->count);
        }
        else {
            printf("Couldn't find..\n");
        }

        printf("Search for key2\n");
        StatData* found2 = ht_search(ht, key2);
        if (found2) {
            assert(found2->id == atol(key2));
            printf("Found data with id %s: count=%d\n", key2, found2->count);
        }
        else {
            printf("Couldn't find..\n");
        }

        free_table(ht);
    }


    printf("\nJoinDump() Test\n");
    // Fast test joinDump
    {
        size_t new_data_len = 0;
        StatData* newData = JoinDump(res_array, 7, res_array, 7, &new_data_len);
        SortDump(newData, new_data_len);
        PrintDump(newData, new_data_len);
        free(newData);
    }

    free(res_array);

	printf("bye there\n");
	return 0;
}

