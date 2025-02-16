#ifndef _STAT_DATA_
#define _STAT_DATA_

#define HASH_KEY_SIZE 20

//TODO: add CRC/hashing of dump for additional security  

typedef struct StatData {
	long id;
	int count;
	float cost;
	unsigned int primary:1;
	unsigned int mode:3;
} StatData;

int compareStatData(const void* a, const void* b);

void SortDump(StatData* array, size_t len);

StatData* JoinDump(StatData* a, size_t a_len, StatData* b, size_t b_len, size_t*);

// Writes 1) len of data_arr for additional security, than writes each packet in data_arr
int StoreDump(const StatData* const data_arr, size_t len, const char* filename);

StatData* LoadDump(const char* filename, size_t* return_size);

void PrintDump(const StatData* data_array, size_t array_size);

void mergeDumpItems(StatData* old, StatData*);

#endif

