#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>
#include "helper.h"
#include "settings.h"

int main(int argsc, char **argsv)
{
    if (argsc > 3 || argsc < 2)
    {
        fprintf(stderr, "Invalid amount of Arguments\n");
        printHelp();
        return -1;
    }

    if (!strcmp(argsv[1], "-h") || !strcmp(argsv[1], "--help"))
    {
        printHelp();
        return 0;
    }
    bool validArgs = checkArgs(argsv);
    if (!validArgs)
    {
        return -1;
    }
    else
    {
        // TODO CLOSE FILES ON RETURN
        // In this else statement we assume that arguments are valid
        FILE *inputFile = fopen(argsv[1], "r");
        FILE *outputFile = fopen(argsv[2], "w");
        if (inputFile != NULL || outputFile != NULL)
        {
            int numOfLines = 0;
            char *line;
            size_t len = 0;
            while (getline(&line, &len, inputFile) != -1)
            {
                if (numOfLines > MEMORY_SIZE)
                {
                    fprintf(stderr, "ERROR: Memory Overflow\n");
                    fprintf(stderr, "MAX MEMORY IS %d\n", MEMORY_SIZE);
                    return 1;
                }
                if (!parseLine(line, numOfLines, outputFile))
                {
                    return false;
                }
                numOfLines++;
            }
        }
        else
        {
            fprintf(stderr, "Error opening files\n");
            return -1;
        }
    }
}
