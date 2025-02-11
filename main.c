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

int StoreDump(const StatData* const data_arr, uint64_t len, const char* filename)
{
	FILE* outfile = fopen(filename, "wb");  // Use "wb" for binary mode
	if (outfile == NULL) {
		fprintf(stderr, "\nError opening file\n");
		return 1;  // Exit with an error code
	}

	uint64_t size_written = fwrite(&len, sizeof(len), 1, outfile);

	if(size_written != 1){
	    fprintf(stderr, "Error writing to file.  Wrote %zu elements, expected 1\n",
		size_written);
	    abort();
	}

	// Write the entire array of structures to the file
	uint64_t elements_written = fwrite(data_arr, sizeof(StatData), len, outfile);

	if (elements_written == len) {
		printf("Successfully wrote the array of structures to the file.\n");
	} else {
		fprintf(stderr, "Error writing to file.  Wrote %zu elements, expected %ld\n",
		elements_written, len);
		abort();
	}

	// Close the file
	fclose(outfile);
	return 0;
}

StatData* LoadDump(const char* filename, uint64_t* return_size)
{
    size_t n = sizeof(StatData);  // Number of bytes to read in each iteration
    unsigned char buffer[n];               // Buffer to store the read bytes
    size_t bytes_read;

    FILE* fp = fopen(filename, "rb"); // Open in binary read mode
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    bytes_read = fread(buffer, 1, sizeof(uint64_t), fp);

    if(bytes_read != sizeof(uint64_t)){
	printf("Error reading length of file");
	return NULL;
    }

    uint64_t len_of_file = 0;
    memcpy(&len_of_file, buffer, sizeof(len_of_file));
    unsigned char* res = (unsigned char*)malloc(sizeof(StatData) * len_of_file);
    printf("Length of file: %ld\n", len_of_file);

    uint64_t offset = 0;

    // Iteratively read from the file
    while ((bytes_read = fread(buffer, 1, n, fp)) > 0) {
        // Process the data in the buffer
        printf("Read %zu bytes:\n", bytes_read);

        // Example: Print the bytes as hexadecimal values
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%.2X ", (unsigned char)buffer[i]);
        }
        printf("\n");

	if(bytes_read == n)
	{
	    printf("Copying\n");
	    memcpy(res+offset, buffer, n); 
	    offset+=bytes_read;
	}
    }

    if (ferror(fp)) {
        perror("Error reading file");
    }

    fclose(fp);

    *return_size = offset/n;
    return (StatData*)res;	
}

void PrintDump(const StatData* data_array, uint64_t array_size){
    printf("Printing dump %ld\n", array_size);
    for (int i = 0; i < array_size; i++) {
        printf("Data %d: id=%ld, count=%d, cost=%.2f, primary=%u, mode=%u\n",
               i, data_array[i].id, data_array[i].count, data_array[i].cost,
               data_array[i].primary, data_array[i].mode);
    }
}

int main(){
	printf("hello there\n");
	const StatData case_1_in_a[7] =
	{
	{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
	};

	const StatData case_1_in_b[2] =
	{{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
	{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};

	/* Ожидаемый результат обработки */
	const StatData case_1_out[3] =
	{{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2 },
	{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3 },
	{.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2 }};

	StoreDump(case_1_in_a, 7, "statdata.bin");

	uint64_t len = 0;
	StatData* res_array = LoadDump("statdata.bin", &len);
	PrintDump(res_array, len);

	printf("bye there\n");
	return 0;
}

