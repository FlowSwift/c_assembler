#include "line_parser.h"

/*TO DO:
    - struct defintion - V
    - add label handeling here?
            check if OK
            add to symbolTable
    - add determine lines
*/
const char *entries[] = {".entry", ".extern", ".data", ".string"};
const Opcode OPCODES[] = {
    {0, "mov", 2},
    {1, "cmp", 2},
    {2, "add", 2},
    {3, "sub", 2},
    {4, "not", 1},
    {5, "clr", 1},
    {6, "lea", 2},
    {7, "inc", 1},
    {8, "dec", 1},
    {9, "jmp", 1},
    {10, "bne", 1},
    {11, "red", 1},
    {12, "prn", 1},
    {13, "jsr", 1},
    {14, "rts", 0},
    {15, "hlt", 0}};

const char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*every line will be parsed and inputed to this structure */
AssemblyLine* parseAssemblyLine(const char *line)
{    
    char *colonPos = NULL;
    char *spacePos = NULL;
    AssemblyLine parsedLine;
    /*Initialize the parsedLine fields*/
    parsedLine.label = NULL;
    parsedLine.instruction = NULL;
    parsedLine.srcOperand = NULL;
    parsedLine.destOperand = NULL;

    colonPos = strchr(line, ':');
    if (colonPos != NULL)
    {
        /* --- Setting label --- */
        size_t labelLen = colonPos - line;
        parsedLine.label = (char *)malloc(labelLen + 1);
        strncpy(parsedLine.label, line, labelLen);
        parsedLine.label[labelLen] = '\0';

        /* Move the pointer past the colon and any whitespace */
        line = colonPos + 1;
        while (*line == ' ')
            line++;
    }
    /* --- Setting instruction ---*/
    spacePos = strchr(line, ' ');
    if (spacePos != NULL)
    {
        size_t instrLen = spacePos - line;
        parsedLine.instruction = (char *)malloc(instrLen + 1);
        strncpy(parsedLine.instruction, line, instrLen);
        parsedLine.instruction[instrLen] = '\0';

        /* Move the pointer past the space */
        line = spacePos + 1;
    }
    else
    {
        /* If there is no space, the entire line is the instruction and no operands, e.g: END: hlt */
        parsedLine.instruction = strdup(line);
        return parsedLine;
    }
    /* --- Setting operands --- */
    parsedLine.operands = strdup(line);
    return &parsedLine;
}

void freeAssemblyLine(AssemblyLine *line)
{
    if (line != NULL)
    {
        free(line->label);
        free(line->instruction);
        free(line->operands);
        if (line->srcOperand != NULL)
        {
            freeOperand(line->src);
        }
        if (line->destOperand != NULL)
        {
            freeOperand(line->dst);
        }
        line = NULL;
    }
}

void freeOperand(Operand *operand)
{
    if (operand != NULL)
    {
        free(operand->value);
        free(operand);
        operand = NULL;
    }
}