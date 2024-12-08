#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "input.txt"
#define LINE_SIZE 1024 // Examining puzzle shows that 1024 is more than sufficient for a line length.

int main() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("Can't open file.");
        return 1;
    }

    char *linePointer = malloc(LINE_SIZE);
    if (linePointer == NULL) {
        perror("Couldn't create a line pointer.");
        fclose(fp);
        return 1;
    }

    size_t bytesRead = 0;

    while (fgets(linePointer, LINE_SIZE, fp) != NULL) {
        size_t lineLength = strlen(linePointer);
        bytesRead += lineLength;
        printf("Got a line = %s", linePointer);
        printf("Line length = %zu\n************************\n", lineLength);
    }

    printf("Bytes Read = %zu\n", bytesRead);

    free(linePointer);
    fclose(fp);
    return 0;
}
