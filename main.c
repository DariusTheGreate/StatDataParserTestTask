#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"
#include "statData.h"

// Todo:
// - rename ht functions
// - test framework cariables
// - Fix bug from valgrind
// - Review 
// - Why prints Success
// - Refactor print


int main(int argc, char *argv[]) {	
    if(argc < 4){
        perror("Invalid number of arguments.");
        return -1;
    }
    
    size_t first_dump_len = 0;
    StatData* ld1 = LoadDump(argv[1], &first_dump_len);
 
    size_t second_dump_len = 0;
    StatData* ld2 = LoadDump(argv[2], &second_dump_len);

    size_t joined_len = 0;
    StatData* joined = JoinDump(ld1, first_dump_len, ld2, second_dump_len, &joined_len);

    SortDump(joined, joined_len);

    PrintDump(joined, 10);

    StoreDump(joined, joined_len, argv[3]);
    
	return 0;
}

