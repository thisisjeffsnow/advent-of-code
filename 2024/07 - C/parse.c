#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define FILE_NAME "input.txt"
#define LINE_SIZE 1024 // Puzzle lines don't exceed this size.
// Probably better to dyanmically figure out the longest line. This
// might be a security problem if it was used in a real project. I'm not sure.
// It would be worth investigating though.

int main() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("Can't open file.");
        return 1;
    }

    char *lp = malloc(LINE_SIZE);
    if (lp == NULL) {
        perror("Couldn't create a line pointer.");
        fclose(fp);
        return 1;
    }

    size_t bytesRead = 0;

    while (fgets(lp, LINE_SIZE, fp) != NULL) {
        size_t lineLength = strlen(lp);
        bytesRead += lineLength;
        printf("Got a line = %s", lp);
        //printf("Line length = %zu\n", lineLength);

        // Alright, I figured out how to grab lines. Can we separate the numbers?
        // AAAA: B C D E F G .....

        // Find the colon.
        char *colonIndex = strchr(lp, ':');
        if(colonIndex == NULL) {
            perror("Found a line without a colon. Exiting.");
            fclose(fp);
            return 1;
        }

        // Grab the left number. Change the colon to string terminator.
        *colonIndex = '\0';
        // Puzzle shows that the left number is always positive, so use unsigned and a big storage for number.
        uint64_t leftNumber = strtoull(lp, NULL, 10); 
        // Test
        printf("Left Number = %" PRIu64 "\n", leftNumber);
        // printf("**********\n\n");

        // Alright, let's get the rest of them.
        // Move to the right hand side of the colon, past the string terminator.
        char *rightSide = colonIndex + 1;

        // All of them have spaces between them, so split them.
        char *piece = strtok(rightSide, " ");
        if(piece != NULL) {
            uint64_t rightNumber = strtoull(piece, NULL, 10);
            printf("Right number = %" PRIu64 "\n", rightNumber);
            printf("**********\n\n");
        } else {
            perror("Couldn't grab number on the right. Failure.");
            fclose(fp);
            return 1;
        }





    }

    printf("Bytes Read = %zu\n", bytesRead);

    free(lp);
    fclose(fp);
    return 0;
}
