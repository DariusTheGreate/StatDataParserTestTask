#ifndef _HASH_TABLE_
#define _HASH_TABLE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "statData.h"

typedef struct Ht_item {
    char* key;            // String representation of the id
    StatData* value;      // Pointer to the StatData
    struct Ht_item* next; // For chaining
    uint8_t dirty;           // Mark every created item
} Ht_item;

Ht_item* create_item(const char* key, void* value);

void free_item(Ht_item* item);

typedef struct HashTable {
    Ht_item** items;   // Array of pointers to Ht_item
    int size;      // Capacity of the hash table
    int count;     // Number of elements currently stored
} HashTable;

HashTable* create_table(int size);

void free_table(HashTable* table);


unsigned long hash_function(long id, int capacity);

void ht_insert(HashTable* table, char* key, StatData* value);

void ht_insert_stat_data(HashTable* table, StatData* data);

StatData* ht_search(HashTable* table, char* key);

void ht_delete(HashTable* table, char* key);


void ht_iterate(HashTable* table, void (*callback)(Ht_item*));

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht);

#endif

