#ifndef _STAT_DATA_
#define _STAT_DATA_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct StatData {
	long id;
	int count;
	float cost;
	unsigned int primary:1;
	unsigned int mode:3;
} StatData;

int compareStatData(const void *a, const void *b);

void SortDump(const StatData* array, size_t len);

StatData* JoinDump(StatData* a, size_t a_len, StatData* b, size_t b_len, size_t*);

int StoreDump(const StatData* const data_arr, size_t len, const char* filename);

StatData* LoadDump(const char* filename, size_t* return_size);

void PrintDump(const StatData* data_array, size_t array_size);

#endif

