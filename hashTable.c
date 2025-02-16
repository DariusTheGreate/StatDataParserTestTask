#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"

#define MAX_HT_SIZE 1000000000

HtItem* create_item(const char* key, void* value) {
    HtItem* item = (HtItem*)malloc(sizeof(HtItem));
    if(!item){
        perror("hashTable.c CreateItem()::Failed to allocate memory for HtItem");
        return NULL;
    }

    item->key = (char*)malloc(strlen(key) + 1);
    if(!item->key){
        perror("hashTable.c CreateItem()::Failed to allocate memory for HtItem key");
        free(item);
        return NULL;
    }

    strcpy(item->key, key);

    item->value = value; // Store the pointer directly
    item->next = NULL;   // Initialize next pointer
    return item;
}

void free_item(HtItem* item) {
    if(item && item->key){
        free(item->key);
    }
    free(item);
}

HashTable* create_table(int size) {
    if(size > MAX_HT_SIZE || size < 0) {
        perror("hashTable.c CreateTable()::Invalid size of table");
        return NULL;
    }

    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
     if(!table){
        perror("hashTable.c CreateTable()::Failed to allocate memory");
        return NULL;
    }
    table->size = size;
    table->count = 0;
    table->items = (HtItem**)calloc(table->size, sizeof(HtItem*));
     if(!table->items){
        perror("hashTable.c CreateTable()::calloc failed");
        free(table);
        return NULL;
    }
    return table;
}

void free_table(HashTable* table) {
    if(table){
        if(table->items){
            for (size_t i = 0; i < table->size; i++) {
                HtItem* item = table->items[i];
                while (item != NULL) {
                    HtItem* next = item->next;
                    free_item(item);
                    item = next;
                }
            }
            free(table->items);
        }
        free(table);
    }
}

uint64_t hash_function(long id, int capacity) {
    //FNV-1a hash algorithm
    uint64_t hash = 0xcbf29ce484222325;
    uint8_t* key = (unsigned char*)&id;
    for(size_t i = 0; i < sizeof(long); i++){
        hash ^= key[i];
        hash *= 0x100000001b3; // for 32bit better use other number 
    }
    return hash % capacity;
}

uint64_t hash_function_fast(long id, int capacity) {
    uint64_t hash = 5381; 

    uint8_t* key = (unsigned char*)&id;
    for (size_t i = 0; i < sizeof(long); i++) {
        hash = ((hash * 33) + hash) + key[i];
    }

    return hash % capacity;
}

void ht_insert(HashTable* table, char* key, StatData* value) {
    if(!table || !value || !table->items) {
        return;
    }

    int index = hash_function(atol(key), table->size);
    //assert(index > 0 && index < table->size);
    HtItem* item = create_item(key, value);
    assert(item);

    if(!table->items[index]){
        //No collision, set the item
        table->items[index] = item;
        table->count++;
    } else {
        //Collision! Collect branch list!
        item->next = table->items[index];
        table->items[index] = item;
        table->count++;
    }
}

void ht_merge_key(HashTable* table, char* key, StatData* value, void (*merge_callback)(StatData*, StatData*)) {
    if(!table || !value || !table->items || key == NULL) {
        return;
    }

    int index = hash_function(atol(key), table->size);  
    //assert(index > 0 && index < table->size);
    HtItem* item = create_item(key, value);
    if(!item) {
        perror("Unable to create_item in ht_merge_key()");
        return;
    }

    if(!table->items[index]) {
        //No collision, create the item
        table->items[index] = item;
        table->count++;
    } 
    else {
        //Iterate branch:
        HtItem* iter = table->items[index];
        while(iter != NULL) {
            // Collision, in case of duplicate id - merge keys via merge_callback.
            if(strcmp(key, iter->key) == 0) { 
                //merge_callback(table->items[index]->value, item->value);
                merge_callback(iter->value, item->value);
                free_item(item);// We dont need this item after merge.
                return;
            }              
            iter = iter->next;
        }

        // Add new item to branch in case theres no elements to merge with.
        item->next = table->items[index];
        table->items[index] = item;
        table->count++;
    }
}

void ht_insert_stat_data(HashTable* table, StatData* data) {
    char key[KEY_BUFFER_LEN]; //Sufficiently large buffer for the long
    snprintf(key, sizeof(key), "%ld", data->id);
    ht_insert(table, key, data);
}

void ht_merge_stat_data(HashTable* table, StatData* data, void (*merge_callback)(StatData*, StatData*)) {
    char key[KEY_BUFFER_LEN]; //Sufficiently large buffer for the long
    snprintf(key, sizeof(key), "%ld", data->id);
    ht_merge_key(table, key, data, merge_callback);
}

StatData* ht_search(HashTable* table, char* key) {
    if(!table || !table->items) {
        return NULL;
    }
    int index = hash_function(atol(key), table->size); 
    //assert(index > 0 && index < size);
    HtItem* item = table->items[index];
    while(item != NULL)
    {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
        item = item->next;
    } 
    return NULL;
}

uint8_t ht_delete(HashTable* table, char* key) {
    if(!table || !table->items)
        return -1;

    int index = hash_function(atol(key), table->size);
    //assert(index > 0 && index < size);
    HtItem* current = table->items[index];
    HtItem* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->items[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free_item(current);
            table->count--;
            return 0;
        }

        prev = current;
        current = current->next;
    }

    return -1;
}

void ht_iterate(HashTable* table, void (*callback)(HtItem*)) {
    if (!table || !table->items || !callback) {
        return; 
    }

    for (size_t i = 0; i < table->size; i++) {
        HtItem* current = table->items[i];
        while (current != NULL) {
            callback(current);
            current = current->next;
        }
    }
}

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht)
{
    for (size_t i = 0; i < array_size; i++) {
	    ht_merge_stat_data(ht, &data_array[i], mergeDumpItems);
    }
}

