#include <stdio.h>
#include <string.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
/*FLOW:
    - read line after line, count number of line -
    - parse the line to 3 sections - label(optional), instucrion, operandes
    - for every parsed line, see if it is allowed by a specific function
        - label:
            - is legal
            - if directive line - check if its legal
                - parse operandes accordinly - if string if ...
            - up ID
            - if everything legal - check if already in symbol table (if yes, ERROR), if not - ADD by IC,
                continue, if legal by checking (OperandeCheck).
            - parse operandes.
                if operand is also label ?
            - check if
            - write in Binary structure into Binary list
        - not label -
            - if start with . -> ? (i think end reading)
            - if regular opcode:
                - check how many operandes to receive
                - find number represenation of Opcode
                - parse operands by ',' - if number as many as supposed to be - else ERROR
                    - check type of miun - see if its allowed for this operand
                    - make switch case on what to do binary for every case and edge case 3
                - if operand register - make binary
                - if operand is LABEL - write name only (?)
*/
int firstPass(char *file_name, SymbolTable *symbolTable, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    FILE *amfile = fopen(file_name, "r");
    AssemblyLine parsedLine;
    /* SymbolNode *current; */
    int lineNumber = 0;
    /* errorCode = SUCCESS; */
    /* FILE *file = NULL; */
    /* File file; */
    while (fgets(line, MAX_LINE_LENGTH + 1, amfile) != NULL)
    {
        lineNumber++;
        if (line[strlen(line) - 1] != '\n')
        {
            /* error handeling */
        }
        parsedLine = parseAssemblyLine(line);
        printf("Parsed Line:\n");
        printAssemblyLine(&parsedLine);
        /* make all the checks if allowed */
        /*if (ParsedLine->label!=NULL){ }*/
        /* all the checks if legal will be here */
        freeAssemblyLine(&parsedLine);
    }
    return 0;
}

int is_directive(AssemblyLine* parsedLine)
{
    /* Check if the first character of the instruction is a dot, as should be in directive ('.')*/
    return (parsedLine->instruction[0] == '.') ? 1 : 0;
}

int countOccurrences(const char *str, char ch) {
    int count = 0;
    while (*str) {
        if (*str == ch) {
            count++;
        }
        str++;
    }
    return count;
}

int check_type(Operand *operand){
    switch (operand->value[0]) 
    {
        case '*':
            operand->type = 2;
            break;
        case '#':
            operand->type = 0;
            break;
        case 'r':
            if (strlen(operand->value) == 2 && operand->value[1] >= '0' && operand->value[1] <= '7') {
                operand->type = 3;
                break;
            }
            else {
                /* IMPROVE ERRORS*/
                /*supposed to be register but not */
                return 0;
            }
        default:
            operand->type = 1; /* Default is it's label, will be checked in 2 pass */
            break;
    }
    return 1;
}

int operand_parser(AssemblyLine* parsedLine){
    /*
    TO DO - 
        - check if up to 2 - V
        - check type of miun - V
        - change miun to type - V
        - insert value to Operand structure - V
    */
    char *ptr_in_line = NULL;
    ptr_in_line = parsedLine->operands;
    Operand *srcOperand = NULL, *destOperand = NULL;
    int srcLen = 0, destLen = 0;
    int operandCount = 0;

    srcOperand = (Operand *)malloc(sizeof(Operand));
    destOperand = (Operand *)malloc(sizeof(Operand));
    srcOperand->value = NULL;
    destOperand->value = NULL;
    
    if (*ptr_in_line != '\0') {
        char *start = ptr_in_line;
        while (*ptr_in_line != '\0' && *ptr_in_line != ',') {
            ptr_in_line++;
            srcLen++;
        }
        srcOperand->value = (char *)malloc(srcLen + 1);
        strncpy(srcOperand->value, start, srcLen);
        srcOperand->value[srcLen] = '\0';
        if (!check_type(srcOperand)) {
            /* IMPROVE ERRORS */
            free(srcOperand->value);
            free(destOperand);
            free(srcOperand);
            return 0;
        }
        operandCount++;
        if (*ptr_in_line == ',') ptr_in_line++; // Skip comma
        while (*ptr_in_line != '\0' && isspace(*ptr_in_line)) ptr_in_line++; // Skip any whitespace between operands
    }
    /*Parse second operand*/
    if (*ptr_in_line != '\0') {
        char *start = ptr_in_line;
        while (*ptr_in_line != '\0' && *ptr_in_line != ',') {
            ptr_in_line++;
            destLen++;
        }
        destOperand->value = (char *)malloc(destLen + 1);
        strncpy(destOperand->value, start, destLen);
        destOperand->value[destLen] = '\0';
        if (!check_type(destOperand)) {
            /* IMPROVE ERRORS */
            free(srcOperand->value);
            free(destOperand->value);
            free(destOperand);
            free(srcOperand);
            return 0;
        }
        operandCount++;
        if (*ptr_in_line == ',') ptr_in_line++; /*Skip comma*/
    }
    /*Check for extra characters after second operand*/
    while (*ptr_in_line != '\0') {
        if (!isspace(*ptr_in_line)) {
            /* IMPROVE ERRORS */
            /* there are extra chars after the second operand */
            if (srcOperand->value) free(srcOperand->value);
            if (destOperand->value) free(destOperand->value);
            free(srcOperand);
            free(destOperand);
            return 0;
        }
        ptr_in_line++;
    }
    /* Assign operands to parsedLine*/
    parsedLine->srcOperand = (operandCount > 0) ? srcOperand : NULL;
    parsedLine->destOperand = (operandCount > 1) ? destOperand : NULL;
    return 1;
}