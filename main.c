#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "statData.h"

// Todo:
// - Review 
// - Add readme
// - make size_t in ht
// - rename htitem

static inline size_t MIN(size_t x, size_t y) {
    return (x > y) ? x : y;
}

int main(int argc, char *argv[]) {	
    if(argc < 4){
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }
    
    size_t first_dump_len = 0;
    StatData* ld1 = LoadDump(argv[1], &first_dump_len);
    if(!ld1) {
        fprintf(stderr, "Error loading dump %s\n", argv[1]);
        return -1;
    }
 
    size_t second_dump_len = 0;
    StatData* ld2 = LoadDump(argv[2], &second_dump_len);
    if(!ld2) {
        fprintf(stderr, "Error loading dump %s\n", argv[2]);
        return -1;
    }

    size_t joined_len = 0;
    StatData* joined = JoinDump(ld1, first_dump_len, ld2, second_dump_len, &joined_len);
    if(!joined) {
        fprintf(stderr, "Error joning the dump at %s and %s.\n", argv[1], argv[2]);
        return -2;
    }

    SortDump(joined, joined_len);

    PrintDump(joined, MIN(10, joined_len));

    if(StoreDump(joined, joined_len, argv[3]) != 0)
        fprintf(stderr, "Error storing joined data.");


    free(ld1);
    free(ld2);
    free(joined);
    
	return 0;
}

