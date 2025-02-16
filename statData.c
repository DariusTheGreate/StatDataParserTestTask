#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "statData.h"
#include "hashTable.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int compareStatData(const void* a, const void* b) {
    StatData *dataA = (StatData *)a;
    StatData *dataB = (StatData *)b;
    if (dataA->cost < dataB->cost) return -1;
    if (dataA->cost > dataB->cost) return 1;
    return 0;
}

void SortDump(StatData* array, size_t len)
{
    qsort((StatData*)array, len, sizeof(StatData), compareStatData);
}

void mergeDumpItems(StatData* old, StatData* new) {
    if(!old || !new)
        return;

    old->count += new->count;    
    old->cost  += new->cost;    
    old->primary &= new->primary;
    old->mode = MAX(new->mode, old->mode);
}

StatData* JoinDump(StatData* a, size_t a_len, StatData* b, size_t b_len, size_t* result_len) {
    HashTable* ht = create_table(a_len + b_len); // Worst case we store a_len + b_len elements.
    if(!ht) {
        perror("statData.c JoinDump()::Failed to create hash table in JoinDump.");
        return NULL;
    }

    // Add items from Dump b into table. 
    // Method InsertDumpToHT merges datas with simmilar ids.  
    InsertDumpToHT(a, a_len, ht);

    // Add values from b.
    for (size_t i = 0; i < b_len; i++) {
        char key[HASH_KEY_SIZE];
        snprintf(key, sizeof(key), "%ld", b[i].id);

        StatData* found = ht_search(ht, key);
        if(found != NULL) {
            char key_found[HASH_KEY_SIZE];
            snprintf(key_found, sizeof(key_found), "%ld", found->id);

            if(strcmp(key, key_found) == 0) {
                mergeDumpItems(found, &b[i]);
            }

        } 
        else {
            ht_insert(ht, key, &b[i]);
            //ht_merge_key(ht, key, &b[i], mergeDumpItems);
        }
    }

    // Count number of items, before allocation and allocate.
    *result_len = ht->count;//countItems(ht);
    StatData* res = (StatData*)malloc(sizeof(StatData) * (*result_len));
    if(!res){
        perror("statData.c JoinDump()::Failed to allocate memory for result Dump in JoinDump.");
        return NULL;
    }

    // Iterate through table and copy elements to result array.
    size_t resI = 0;
    for (int i = 0; i < ht->size; i++) {
        Ht_item* current = ht->items[i];
        while (current != NULL) {
            memcpy(&res[resI++], current->value, sizeof(StatData));
            current = current->next;
        }
    }
    
    free_table(ht);

    return res;
}

int StoreDump(const StatData* const data_arr, size_t len, const char* filename)
{
    if(!data_arr) {
        return -4;
    }

	FILE* outfile = fopen(filename, "wb"); 
	if (!outfile) {
	    perror("Error opening file");
	    return -1; 
	}

    // Explicitly write amount of Dump packets we store, for additional security.
	size_t size_written = fwrite(&len, sizeof(len), 1, outfile);

	if(size_written != 1){
	    perror("statData.c StoreDump()::Error writing size of Dump to file.");
	    fclose(outfile);
	    return -2;
	}

	size_t elements_written = fwrite(data_arr, sizeof(StatData), len, outfile);

	if(elements_written != len){
	    perror("statData.c StoreDump():: Error writing Dump to file.");
	    fclose(outfile);
	    return -3;
	}

	fclose(outfile);
	return 0;
}

StatData* LoadDump(const char* filename, size_t* return_size)
{
    uint8_t buffer[sizeof(StatData)];

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("statData.c LoadDump()::Error opening file\n");
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, sizeof(size_t), fp);

    if(bytes_read != sizeof(size_t)){
        perror("statData.c LoadDump()::Error reading length of file");
        return NULL;
    }

    size_t amount_of_packets = 0;
    memcpy(&amount_of_packets, buffer, sizeof(amount_of_packets));
    uint8_t* res = (uint8_t*)malloc(sizeof(StatData) * amount_of_packets);
    if(!res) { 
        perror("statData.c LoadDump()::Error reading length of file");
        return NULL;
    }

    size_t offset = 0;
    while ((bytes_read = fread(buffer, 1, sizeof(StatData), fp)) > 0) {
        if(bytes_read == sizeof(StatData)) {
            memcpy(res+offset, buffer, sizeof(StatData)); 
            offset+=bytes_read;
        }
        else {
            perror("statData.c LoadDump()::Error reading file\n");
            break;    
        }
    }

    if (ferror(fp)) {
        perror("statData.c LoadDump()::Error reading file\n");
    }

    fclose(fp);

    *return_size = offset/sizeof(StatData);
    return (StatData*)res;	
}

void PrintBinary(unsigned int n) {
    // Assuming a 32-bit unsigned integer
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        putchar((n & (1U << i)) ? '1' : '0');
    }
    putchar('\n'); // New line after printing the binary number
}

void PrintDump(const StatData* data_array, size_t array_size){
    for (size_t i = 0; i < array_size; i++) {
        printf("Data %ld: id=%lx, count=%d, cost=%e, primary=%s, mode=",
               i, data_array[i].id, data_array[i].count, data_array[i].cost,
               data_array[i].primary == 1 ? "y" : "n");
        PrintBinary(data_array[i].mode);
    }
}

