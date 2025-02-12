#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "statData.h"
#include "hashTable.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int compareStatData(const void *a, const void *b) {
    StatData *dataA = (StatData *)a;
    StatData *dataB = (StatData *)b;
    if (dataA->cost < dataB->cost) return -1;
    if (dataA->cost > dataB->cost) return 1;
    return 0;
}

void SortDump(const StatData* array, size_t len)
{
    qsort((StatData*)array, len, sizeof(StatData), compareStatData);
}

StatData* JoinDump(StatData* a, size_t a_len, StatData* b, size_t b_len, size_t* result_len)
{
    // use HashTable? O(n) + O(n) + O(n)
    // sort each of those and combine? O(n*logn) * 2 + O(n) 
    // bruteforce? O(n^2)
    
    HashTable* ht = create_table(a_len + b_len);
    InsertDumpToHT(a, a_len, ht);
    
    for (size_t i = 0; i < b_len; i++) {
        char key[20];
        snprintf(key, sizeof(key), "%ld", b[i].id);
        StatData* found = ht_search(ht,key);
        if(found) {
            found->count += b[i].count;    
            found->cost  += b[i].cost;    
            found->primary = (found->primary | b[i].primary);
            found->mode = MAX(found->mode, b[i].mode);
        } 
        else{
            ht_insert(ht, key, &b[i]);
        }
    }

    size_t count_items = 0;
    for (int i = 0; i < ht->size; i++) {
        Ht_item* current = ht->items[i];
        while (current != NULL && current->dirty) {
            count_items++;
            current = current->next;
        }
    }

    *result_len = count_items;

    StatData* res = (StatData*)malloc(sizeof(StatData) * count_items);
    if(!res){
        perror("Failed to allocate memory for result Dump in JoinDump.");
        free(res);
        return res;
    }

    size_t resI = 0;
    for (int i = 0; i < ht->size; i++) {
        Ht_item* current = ht->items[i];
        while (current != NULL && current->dirty) {
            memcpy(&res[resI++], current->value, sizeof(StatData));
            current = current->next;
        }
    }
    
    free_table(ht);

    return res;
}

int StoreDump(const StatData* const data_arr, size_t len, const char* filename)
{
	FILE* outfile = fopen(filename, "wb"); 
	if (!outfile) {
	    perror("Error opening file");
	    fclose(outfile);
	    return -1; 
	}

	size_t size_written = fwrite(&len, sizeof(len), 1, outfile);

	if(size_written != 1){
	    perror("Error writing size to file.");
	    fclose(outfile);
	    return -2;
	}

	size_t elements_written = fwrite(data_arr, sizeof(StatData), len, outfile);

	if(elements_written != len){
	    perror("Error writing length of dump to file.");
	    fclose(outfile);
	    return -3;
	}

	fclose(outfile);
	return 0;
}

StatData* LoadDump(const char* filename, size_t* return_size)
{
    size_t n = sizeof(StatData);
    unsigned char buffer[n];

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file\n");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, sizeof(size_t), fp);

    if(bytes_read != sizeof(size_t)){
        perror("Error reading length of file");
        fclose(fp);
        return NULL;
    }

    size_t len_of_file = 0;
    memcpy(&len_of_file, buffer, sizeof(len_of_file));
    uint8_t* res = (uint8_t*)malloc(sizeof(StatData) * len_of_file);

    size_t offset = 0;

    while ((bytes_read = fread(buffer, 1, n, fp)) > 0) {
        if(bytes_read == n) {
            memcpy(res+offset, buffer, n); 
            offset+=bytes_read;
        }
        else {
            perror("Error reading file\n");
            break;    
        }
    }

    if (ferror(fp)) {
        perror("Error reading file\n");
    }

    fclose(fp);

    *return_size = offset/n;
    return (StatData*)res;	
}

void PrintDump(const StatData* data_array, size_t array_size){
    printf("Printing dump %ld\n", array_size);
    for (size_t i = 0; i < array_size; i++) {
        printf("Data %ld: id=%ld, count=%d, cost=%.2f, primary=%u, mode=%u\n",
               i, data_array[i].id, data_array[i].count, data_array[i].cost,
               data_array[i].primary, data_array[i].mode);
    }
}

#
