#ifndef _HASH_TABLE_
#define _HASH_TABLE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "statData.h"

#define KEY_BUFFER_LEN 20

typedef struct Ht_item {
    char* key;            // String representation of the id
    StatData* value;      // Pointer to the StatData
    struct Ht_item* next; // For chaining
} Ht_item;

// Caller shouldn't free key string
Ht_item* create_item(const char* key, void* value);

void free_item(Ht_item* item);

typedef struct HashTable {
    Ht_item** items;   // Array of pointers to Ht_item
    int size;      // Capacity of the hash table
    int count;     // Number of elements currently stored
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

void ht_iterate(HashTable* table, void (*callback)(Ht_item*));

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht);

#endif

