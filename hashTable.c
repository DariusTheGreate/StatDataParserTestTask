#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashTable.h"

#define MAX_HT_SIZE 1000000000

Ht_item* create_item(const char* key, void* value) {
    Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
    if(!item){
        perror("hashTable.c CreateItem()::Failed to allocate memory for Ht_item");
        return NULL;
    }

    item->key = (char*)malloc(strlen(key) + 1);
    if(!item->key){
        perror("hashTable.c CreateItem()::Failed to allocate memory for Ht_item key");
        free(item);
        return NULL;
    }

    strcpy(item->key, key);

    item->value = value; // Store the pointer directly
    item->next = NULL;   // Initialize next pointer
    return item;
}

void free_item(Ht_item* item) {
    if(item && item->key){
        free(item->key);
    }
    free(item);
}

HashTable* create_table(int size) {
    if(size > MAX_HT_SIZE || size < 0) {
        perror("hashTable.c CreateTable()::Invaild size of table");
        return NULL;
    }

    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
     if(!table){
        perror("hashTable.c CreateTable()::Failed to allocate memory");
        return NULL;
    }
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));
     if(!table->items){
        perror("hashTable.c CreateTable()::calloc failed");
        free(table);
        return NULL;
    }
    return table;
}

void free_table(HashTable* table) {
    if(table && table->items){
        for (int i = 0; i < table->size; i++) {
            Ht_item* item = table->items[i];
            while (item != NULL) {
                Ht_item* next = item->next;
                free_item(item);
                item = next;
            }
        }
        free(table->items);
        free(table);
    }
}

uint64_t hash_function(long id, int capacity) {
    //FNV-1a hash algorithm
    unsigned long hash = 0xcbf29ce484222325;
    unsigned char* key = (unsigned char*)&id;
    for(size_t i = 0; i < sizeof(long); i++){
        hash ^= key[i];
        hash *= 0x100000001b3;
    }
    return hash % capacity;
}

uint64_t hash_function_fast(long id, int capacity) {
    unsigned long hash = 5381; 

    unsigned char* key = (unsigned char*)&id;
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
    Ht_item* item = create_item(key, value);
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
    if(!table || !value || !table->items) {
        return;
    }

    int index = hash_function(atol(key), table->size);  
    //assert(index > 0 && index < table->size);
    Ht_item* item = create_item(key, value);
    assert(item);

    if(!table->items[index]) {
        //No collision, create the item
        table->items[index] = item;
        table->count++;
    } 
    else {
        //Iterate branch:
        Ht_item* iter = table->items[index];
        uint8_t mergeAccured = 0;
        while(iter != NULL) {
            // Collision, in case of duplicate id - merge keys via merge_callback.
            if(strcmp(key, iter->key) == 0) { 
                mergeAccured = 1;
                //printf("Merge: %s and %s(new) ", table->items[index]->key, key);
                merge_callback(table->items[index]->value, item->value);
                free_item(item);// We dont need this item after merge.
            }              

            iter = iter->next;
        }

        // Add new item to branch in case theres no elements to merge with.
        if(mergeAccured == 0) {
            //printf("Add into branch: %s", item->key);
            item->next = table->items[index];
            table->items[index] = item;
            table->count++;
        }
    }
}

void ht_insert_stat_data(HashTable* table, StatData* data) {
    char key[20]; //Sufficiently large buffer for the long
    snprintf(key, sizeof(key), "%ld", data->id);
    ht_insert(table, key, data);
}

void ht_merge_stat_data(HashTable* table, StatData* data, void (*merge_callback)(StatData*, StatData*)) {
    char key[20]; //Sufficiently large buffer for the long
    snprintf(key, sizeof(key), "%ld", data->id);
    ht_merge_key(table, key, data, merge_callback);
}

StatData* ht_search(HashTable* table, char* key) {
    if(!table || !table->items) {
        return NULL;
    }
    int index = hash_function(atol(key), table->size); 
    //assert(index > 0 && index < size);
    Ht_item* item = table->items[index];
    while(item != NULL)
    {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
        item = item->next;
    } 
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    if(!table || !table->items)
        return;

    int index = hash_function(atol(key), table->size);
    Ht_item* current = table->items[index];
    Ht_item* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->items[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free_item(current);
            table->count--;
            return;
        }

        prev = current;
        current = current->next;
    }
}

size_t countItems(HashTable* ht) {
    if(!ht || !ht->items)
        return 0;

    size_t count_items = 0;
    for (int i = 0; i < ht->size; i++) {
        Ht_item* current = ht->items[i];
        while (current != NULL) {
            count_items++;
            current = current->next;
        }
    }

    return count_items;
}

void ht_iterate(HashTable* table, void (*callback)(Ht_item*)) {
    if (!table || !table->items || !callback) {
        return; 
    }

    for (int i = 0; i < table->size; i++) {
        Ht_item* current = table->items[i];
        while (current != NULL) {
            callback(current);
            current = current->next;
        }
    }
}

int containsDuplicatesCheck(HashTable* table, StatData* a, StatData* b) {
    (void)a;
    (void)b;
    for(int j = 0; j < table->size; ++j) {
        Ht_item* currentJ = table->items[j];
        while(currentJ != NULL) { 
            if(currentJ->value) {
                for (int i = 0; i < table->size; i++) {
                    Ht_item* current = table->items[i];
                    while (current != NULL) {
                        if(current != currentJ && current->value && current->value->id == currentJ->value->id) {
                            printf("Duplicates detected %ld, %ld\n", current->value->id, currentJ->value->id);
                            a = current->value;
                            b = currentJ->value;
                            return 1;
                        }
                        current = current->next;
                    }
                }
            }
            currentJ = currentJ->next;
        }
    }

    return 0;
}

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht)
{
    for (size_t i = 0; i < array_size; i++) {
	    ht_merge_stat_data(ht, &data_array[i], mergeDumpItems);
    }
}

