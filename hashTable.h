#ifndef _HASH_TABLE_
#define _HASH_TABLE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "statData.h"

#define KEY_BUFFER_LEN 20

typedef struct HtItem {
    char* key;            // String representation of the id
    StatData* value;      // Pointer to the StatData
    struct HtItem* next; // For chaining
} HtItem;

// Caller shouldn't free key string
HtItem* create_item(const char* key, void* value);

void free_item(HtItem* item);

typedef struct HashTable {
    HtItem** items;   // Array of pointers to Ht_item
    size_t size;      // Capacity of the hash table
    size_t count;     // Number of elements currently stored
} HashTable;

HashTable* create_table(int size);

void free_table(HashTable* table);

// Depends on length of long
uint64_t hash_function(long id, int capacity);
uint64_t hash_function_fast(long id, int capacity);
// uint64_t hash_murmur(long id, int capacity);

// Function expects that key is valid representation of long!
void ht_insert(HashTable* table, char* key, StatData* value);

// This method is little un contre. 
// Instead of adding collision to the list it perfoms merge_callback on old and new values of key
// This way we can calculate sums, find maximum/minimum, enumerate, etc.
void ht_merge_key(HashTable* table, char* key, StatData* value, void (*merge_callback)(StatData*, StatData*));

void ht_insert_stat_data(HashTable* table, StatData* data);
void ht_merge_stat_data(HashTable* table, StatData* data, void (*merge_callback)(StatData*, StatData*));

StatData* ht_search(HashTable* table, char* key);

uint8_t ht_delete(HashTable* table, char* key);

void ht_iterate(HashTable* table, void (*callback)(HtItem*));

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht);

#endif

