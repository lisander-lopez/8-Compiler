#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include "helper.h"
#include "settings.h"

bool checkArgs(char **args)
{
    if (args[0] == NULL || args[1] == NULL)
    {
        fprintf(stderr, "Arguments are not valid\n");
        printHelp();
        return false;
    }
    else
    {
        // Check if file extensions are correct
        int arg1Len = strlen(args[1]); // .8c
        int arg2Len = strlen(args[2]); // .8b

        if (args[1][arg1Len - 1] == 'c' &&
            args[1][arg1Len - 2] == '8' &&
            args[1][arg1Len - 3] == '.' &&
            args[2][arg2Len - 1] == 'b' &&
            args[2][arg2Len - 2] == '8' &&
            args[2][arg2Len - 3] == '.')
        {
            return true;
        }
        fprintf(stderr, "File Extensions not correct!\n");
        printHelp();
        return false;
    }
}
void printHelp()
{
    printf("----------------------------------------------------\n");
    printf("8 Compiler - A compiler for a niche 8-bit computer\n");
    printf("Version: %.2f \n", VERSION);
    printf("Usage: ./8-bit SOURCECODE.8c OUTPUTFILE.8b\n");
    printf("./8-bit -h || ./8-bit --help (This output)\n");
    printf("----------------------------------------------------\n");
}

bool parseLine(char *line, int lineNumber, FILE *outputFile)
{
    // Line number is using 0 index
    regex_t MAR; // Memory address Regex
    regex_t OCR; // Op code regex
    regex_t ASR; // ARGS regex
    regmatch_t MARrm;
    regmatch_t OCRrm;
    regmatch_t ASRrm;

    regcomp(&MAR, MEMORY_ADDRESS_REGEX, REG_EXTENDED);
    regcomp(&OCR, OP_CODE_REGEX, REG_EXTENDED);
    regcomp(&ASR, ARGS_REGEX, REG_EXTENDED);
    removeNewLineChar(line);
    int marVal = regexec(&MAR, line, 2, &MARrm, 0);
    int ocrVal = regexec(&OCR, line, 2, &OCRrm, 0);
    int asrVal = regexec(&ASR, line, 3, &ASRrm, 0);
    char *memAddress;
    char *opCode;
    char *args;
    if (marVal == 0)
    {
        memAddress = memoryAddressValidator(MAR, &MARrm, line);
        if (ocrVal == 0)
        {
            opCode = opCodeValidator(OCR, &OCRrm, line);
            if ((asrVal != 0) &&
                    (strcmp(lowerCase(opCode), "out") == 0) ||
                (strcmp(lowerCase(opCode), "hlt") == 0) ||
                (strcmp(lowerCase(opCode), "nop") == 0))
            {
                // OUT CODE or Hlt ot NOP
            }
            else
            {
                // Regular operation
                args = argsValidator(ASR, &ASRrm, line);
                // Add memAddress, opcode and args to file
            }
            printf("Mem Address:%s OpCode:%s Args:%s\n", memAddress, opCode, args);
        }
        else
        {
            if (asrVal == 0)
            {
                fprintf(stderr, "Could not parse Operation code on line %d\n", lineNumber + 1);
                return false;
            }
            else
            {
                // Memory space
                printf("Mem Address:%s MEMORY SPACE\n", memAddress);
            }
        }
    }
    else
    {
        fprintf(stderr, "Could not parse Memory address on line %d\n", lineNumber + 1);
        return false;
    }
    return true;
}

char *memoryAddressValidator(regex_t MAR, regmatch_t *rm, char *line)
{
    char *memAddress = calloc(((MEMORY_SIZE / 16) + 3), sizeof(char)); // 0x0-0xf == 16 bytes we need 3 spaces in char + 1 for NUL term, we do, mem_size /16 + 3
    for (int i = rm->rm_so; i < rm->rm_eo; i++)
    {
        memAddress[i - rm->rm_so] = line[i];
    }
    memAddress[rm->rm_eo - rm->rm_so] = '\0';
    return memAddress;
}

char *opCodeValidator(regex_t MAR, regmatch_t *rm, char *line)
{
    char *opCode = calloc(3, sizeof(char)); // OP codes where designed to be 3 chars long
    for (int i = rm->rm_so; i < rm->rm_eo; i++)
    {
        opCode[i - rm->rm_so] = line[i];
    }
    return opCode;
}

char *argsValidator(regex_t MAR, regmatch_t *rm, char *line)
{
    char *args = calloc(((MEMORY_SIZE / 16) + 3), sizeof(char)); // 0x0-0xf == 16 bytes we need 3 spaces in char + 1 for NUL term, we do, mem_size /16 + 3
    for (int i = rm->rm_so; i < rm->rm_eo; i++)
    {
        args[i - rm->rm_so] = line[i];
    }
    return args;
}

static char *lowerCase(char *str)
{
    unsigned char *mystr = (unsigned char *)str;

    while (*mystr)
    {
        *mystr = tolower(*mystr);
        mystr++;
    }
    return str;
}

static void removeNewLineChar(char *line)
{
    int length = strlen(line);
    if (line[length - 1] == '\n')
    {
        line[length - 1] = '\0';
    }
}