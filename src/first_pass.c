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