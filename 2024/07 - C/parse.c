#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "input.txt"
#define CHUNK_SIZE 1024

int main() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("Can't open file.");
        return 1;
    }

    char *puzzleInput = NULL;
    size_t size = 0;
    size_t read = 0;

    int done = 0;

    while (!done) {
        char *newChunk = realloc(puzzleInput, size + CHUNK_SIZE);
        if (newChunk == NULL) {
            perror("Failed to grab chunk from file.");
            free(puzzleInput);
            fclose(fp);
            return 1;
        }

        puzzleInput = newChunk;
        size += CHUNK_SIZE;

        size_t bytesRead = fread(puzzleInput + read, 1, CHUNK_SIZE, fp);
        printf("Bytes read on this pass... %zu (total so far: %zu)\n", bytesRead, read);
        read += bytesRead;

        if (bytesRead < CHUNK_SIZE) {
            if (feof(fp)) {
                done = 1; // end of file
                break;
            } else if (ferror(fp)) {
                perror("Error reading input file.");
                free(puzzleInput);
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);

    if (read == 0) {
        printf("File is empty!\n");
        free(puzzleInput);
        return 0;
    }

    puzzleInput[read] = '\0';

    printf("Total bytes read: %zu\n", read);

    free(puzzleInput);
    return 0;
}
