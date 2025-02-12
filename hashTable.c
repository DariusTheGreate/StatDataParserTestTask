#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"

Ht_item* create_item(const char* key, void* value) {
    Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
    if(!item){
        perror("Failed to allocate memory for Ht_item in create_item");
        return NULL;
    }

    item->key = (char*)malloc(strlen(key) + 1);
    if(!item->key){
        perror("Failed to allocate memory for Ht_item key in create_item");
        free(item);
        return NULL;
    }

    strcpy(item->key, key);

    item->value = value; // Store the pointer directly
    item->next = NULL;   // Initialize next pointer
    item->dirty = 1;     // mark created item

    return item;
}

void free_item(Ht_item* item) {
    if(item && item->key){
        free(item->key);
    }
    free(item);
}

HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
     if(!table){
        perror("malloc failed in create_table");
        return NULL;
    }
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));
     if(!table->items){
        perror("calloc failed in create_table");
        free(table);
        return NULL;
    }
    return table;
}

void free_table(HashTable* table) {
    if(table){
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


unsigned long hash_function(long id, int capacity) {
    //FNV-1a hash algorithm
    unsigned long hash = 0xcbf29ce484222325;
    unsigned char* key = (unsigned char*)&id;
    for(int i = 0; i < sizeof(long); i++){
        hash ^= key[i];
        hash *= 0x100000001b3;
    }
    return hash % capacity;
}

void ht_insert(HashTable* table, char* key, StatData* value) {
    int index = hash_function(atol(key), table->size);  // Use the long id as hash key
    Ht_item* item = create_item(key, value);

    if(!table->items[index]){
        //No collision, set the item
        table->items[index] = item;
        table->count++;
    } else {
        //Collision! Handle this case!
        //Example: Add new item to head of linked list
        item->next = table->items[index];
        table->items[index] = item;
        table->count++;
    }
}

void ht_insert_stat_data(HashTable* table, StatData* data) {
    char key[20]; //Sufficiently large buffer for the long
    snprintf(key, sizeof(key), "%ld", data->id);
    ht_insert(table, key, data);
}

StatData* ht_search(HashTable* table, char* key) {
    int index = hash_function(atol(key), table->size);  // Use the long id as hash key
    Ht_item* item = table->items[index];
    if (item != NULL) {
         if (strcmp(item->key, key) == 0) {
            return item->value;
        }
    }
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    long key_to_delete = atol(key);
    printf("key_to_delete: %ld\n",key_to_delete);
    int index = hash_function(atol(key), table->size);
    Ht_item* current = table->items[index];
    Ht_item* prev = NULL;

    // Traverse the linked list at the index
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found, delete the item
            if (prev == NULL) {
                // If the key is in the first node of the linked list
                table->items[index] = current->next;
            } else {
                // If the key is in the middle or end of the linked list
                prev->next = current->next;
            }

            free_item(current);
            table->count--;
            return; // Deletion successful
        }

        prev = current;
        current = current->next;
    }

    // Key not found (deletion failed - consider logging)
    printf("Warning: Key '%s' not found in hash table for deletion.\n", key);
}

void ht_iterate(HashTable* table, void (*callback)(Ht_item*)) {
    if (!table || !callback) {
        return; // Handle null table or callback
    }

    for (int i = 0; i < table->size; i++) {
        Ht_item* current = table->items[i];
        while (current != NULL) {
            callback(current);
            current = current->next;
        }
    }
}

void InsertDumpToHT(StatData* data_array, size_t array_size, HashTable* ht)
{
    for (size_t i = 0; i < array_size; i++) {
	    ht_insert_stat_data(ht, &data_array[i]);
    }
}


