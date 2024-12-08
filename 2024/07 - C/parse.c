#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define FILE_NAME "input.txt"
#define LINE_SIZE 1024 // Max size of a line in the input file.

// Problem 2: Concatenate operator
uint64_t concatenate(uint64_t left, uint64_t right)
{
    // Convert numbers to strings and concatenate them.
    char leftBuffer[LINE_SIZE];
    char rightBuffer[LINE_SIZE];
    snprintf(leftBuffer, LINE_SIZE, "%" PRIu64, left);
    snprintf(rightBuffer, LINE_SIZE, "%" PRIu64, right);

    strcat(leftBuffer, rightBuffer);

    return strtoull(leftBuffer, NULL, 10);
}

// DFS Helper: Recursively evaluate combinations of +, *, and || to match the left number.
int dfs(uint64_t *rightNumbers, size_t rightNumbersSize, size_t currIndex, uint64_t currentCalculation, uint64_t leftNumber, int problemNumber)
{
    // Base case: If all numbers on the right are used, check if the calculation matches the left number.
    if (currIndex == rightNumbersSize)
    {
        return currentCalculation == leftNumber;
    }

    uint64_t nextRightNumber = rightNumbers[currIndex];

    // Try addition
    if (dfs(rightNumbers, rightNumbersSize, currIndex + 1, currentCalculation + nextRightNumber, leftNumber, problemNumber))
    {
        return 1;
    }

    // Try multiplication
    if (dfs(rightNumbers, rightNumbersSize, currIndex + 1, currentCalculation * nextRightNumber, leftNumber, problemNumber))
    {
        return 1;
    }

    // Try concatenation (only for Part 2)
    if (problemNumber == 2)
    {
        uint64_t concatenated = concatenate(currentCalculation, nextRightNumber);
        if (dfs(rightNumbers, rightNumbersSize, currIndex + 1, concatenated, leftNumber, problemNumber))
        {
            return 1;
        }
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
        perror("Couldn't allocate memory for line pointer.");
        fclose(fp);
        return 1;
    }

    size_t bytesRead = 0;
    uint64_t p1Solution = 0;
    uint64_t p2Solution = 0;

    while (fgets(lp, LINE_SIZE, fp) != NULL)
    {
        size_t lineLength = strlen(lp);
        bytesRead += lineLength;

        // Find the colon to separate the left and right parts of the line.
        char *colonIndex = strchr(lp, ':');
        if (colonIndex == NULL)
        {
            perror("Line missing a colon. Exiting.");
            fclose(fp);
            free(lp);
            return 1;
        }

        // Extract the left number (before the colon).
        *colonIndex = '\0';
        uint64_t leftNumber = strtoull(lp, NULL, 10);

        // Extract the right numbers (after the colon).
        char *rightSide = colonIndex + 1;

        // Parse the right-hand numbers into an array.
        uint64_t rightNumbers[LINE_SIZE];
        size_t currIndex = 0;

        char *piece = strtok(rightSide, " ");
        while (piece != NULL)
        {
            rightNumbers[currIndex] = strtoull(piece, NULL, 10);
            currIndex++;
            piece = strtok(NULL, " ");
        }

        // Solve for Part 1 and Part 2 if there are numbers on the right.
        if (currIndex > 0)
        {
            if (dfs(rightNumbers, currIndex, 1, rightNumbers[0], leftNumber, 1))
            {
                p1Solution += leftNumber;
            }
            if (dfs(rightNumbers, currIndex, 1, rightNumbers[0], leftNumber, 2))
            {
                p2Solution += leftNumber;
            }
        }
    }

    // Output results
    printf("Bytes Read = %zu\n", bytesRead);
    printf("Part 1 Solution = %" PRIu64 "\n", p1Solution);
    printf("Part 2 Solution = %" PRIu64 "\n", p2Solution);

    free(lp);
    fclose(fp);
    return 0;
}
