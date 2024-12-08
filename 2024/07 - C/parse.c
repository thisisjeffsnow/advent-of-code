#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define FILE_NAME "input.txt"
#define LINE_SIZE 1024 // Puzzle lines don't exceed this size.
// Probably better to dyanmically figure out the longest line. This
// might be a security problem if it was used in a real project. I'm not sure.
// It would be worth investigating though.

// DFS Helper for checking all adding and multiplying combinations once we grab all our numbers.
int dfs(uint64_t *rightNumbers, size_t rightNumbersSize, size_t currIndex, uint64_t currentCalculation, uint64_t leftNumber)
{
    // Base Case = We have exhausted all the numbers on the right side, now check that the calcuation is good or not.
    if (currIndex == rightNumbersSize)
    {
        return currentCalculation == leftNumber;
    }

    // Recursion, we need to check addition and multiplication.
    uint64_t nextRightNumber = rightNumbers[currIndex];

    // Add
    if (dfs(rightNumbers, rightNumbersSize, currIndex + 1, currentCalculation + nextRightNumber, leftNumber))
    {
        return 1;
    }

    // Multiply
    if (dfs(rightNumbers, rightNumbersSize, currIndex + 1, currentCalculation * nextRightNumber, leftNumber))
    {
        return 1;
    }

    return 0;
}

int main()
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        perror("Can't open file.");
        return 1;
    }

    char *lp = malloc(LINE_SIZE);
    if (lp == NULL)
    {
        perror("Couldn't create a line pointer.");
        fclose(fp);
        return 1;
    }

    size_t bytesRead = 0;
    uint64_t p1Solution = 0;

    while (fgets(lp, LINE_SIZE, fp) != NULL)
    {
        size_t lineLength = strlen(lp);
        bytesRead += lineLength;
        printf("Got a line = %s", lp);
        // printf("Line length = %zu\n", lineLength);

        // Alright, I figured out how to grab lines. Can we separate the numbers?
        // AAAA: B C D E F G .....

        // Find the colon.
        char *colonIndex = strchr(lp, ':');
        if (colonIndex == NULL)
        {
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

        // // All of them have spaces between them, so split them.

        // For one number:
        // char *piece = strtok(rightSide, " ");
        // if(piece != NULL) {
        //     uint64_t rightNumber = strtoull(piece, NULL, 10);
        //     printf("Right number = %" PRIu64 "\n", rightNumber);
        //     printf("**********\n\n");
        // } else {
        //     perror("Couldn't grab number on the right. Failure.");
        //     fclose(fp);
        //     return 1;
        // }

        // For all numbers on the right:
        uint64_t rightNumbers[LINE_SIZE]; // Very big, but sufficient to hold our numbers in the line.
        // We could theoretically hold every character in the line, let alone spaced out numbers.
        // Find out a better way to do this? Run the line and count spaces to determine array size?
        // Allocate dynamically and expand array when we get close to filling it?
        // Stuff to think about.

        size_t currIndex = 0;

        char *piece = strtok(rightSide, " ");
        while (piece != NULL)
        {
            rightNumbers[currIndex] = strtoull(piece, NULL, 10);
            printf("Grabbed a number! %zu => %" PRIu64 "\n", currIndex, rightNumbers[currIndex]);
            currIndex++;
            piece = strtok(NULL, " ");
        }

        // Now check for solutions. Adding and multiplying our numbers on the right should result in the left number.
        // If there are no numbers on the right, do nothing.
        if (currIndex > 0)
        {
            if (dfs(rightNumbers, currIndex, 1, rightNumbers[0], leftNumber))
            {
                printf("Solution found! %" PRIu64 "\n", leftNumber);
                p1Solution += leftNumber;
            }
        }
    }

    printf("Bytes Read = %zu\n", bytesRead);
    printf("P1 Solution = %" PRIu64 "\n", p1Solution);

    free(lp);
    fclose(fp);
    return 0;
}
