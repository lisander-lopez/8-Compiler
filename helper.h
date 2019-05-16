#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool checkArgs(char **args);
void printHelp();
bool parseLine(char *line, int lineNumber, FILE *outputFile);
char *memoryAddressValidator(regex_t MAR, regmatch_t *rm, char *line);
char *opCodeValidator(regex_t MAR, regmatch_t *rm, char *line);
char *argsValidator(regex_t MAR, regmatch_t *rm, char *line);
static char *lowerCase(char *str);
static void removeNewLineChar(char *line);