#include "line_parser.h"

/*TO DO:
    - struct defintion - V
    - add one word in structure check 
    -  check if malloc succeded
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
AssemblyLine parseAssemblyLine(const char *line) {
     /*call the function with line - like (line) */
    AssemblyLine parsedLine;
    /*Initialize parsed Line structure */
    parsedLine.label = NULL;
    parsedLine.instruction = NULL;
    parsedLine.operands = NULL;
    parsedLine.srcOperand = NULL;
    parsedLine.destOperand = NULL;
	char *spacePos = NULL;
	char *colonPos = NULL;
    /* Find the position of the colon*/
    colonPos = strchr(line, ':');
    if (colonPos != NULL) {
        /* Extract and set the label*/
        size_t labelLen = colonPos - line;
        parsedLine.label = (char *)malloc(labelLen + 1);
        strncpy(parsedLine.label, line, labelLen);
        parsedLine.label[labelLen] = '\0';
        /* Move past the colon*/
        line = colonPos + 1;
        while (*line == ' ') {
                line++;
        }
    }
    /*Find the position of the first space after the label (if any)*/
    spacePos = strchr(line, ' ');
    if (spacePos != NULL) {
        /* Extract and set the instruction*/
        size_t instrLen = spacePos - line;
        parsedLine.instruction = (char *)malloc(instrLen + 1);
        strncpy(parsedLine.instruction, line, instrLen);
        parsedLine.instruction[instrLen] = '\0';
        /* Move past space to operands*/
        line = spacePos + 1;
        /* Set operands*/
        if (*line != '\0') {
                parsedLine.operands = strdup(line);
        } else {
                parsedLine.operands = NULL;
        }
    } else {
        /*If there is no space, the entire line is the instruction with no operands*/
        parsedLine.instruction = strdup(line);
    }
    return parsedLine;
}
void printAssemblyLine2(const AssemblyLine *parsedLine) {
        if (parsedLine == NULL) {
            printf("AssemblyLine is NULL\n");
        return;
    }
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands ? parsedLine->operands : "(none)");
}
int main() {
        char line1[] = "MAIN: add r3, LIST";
    char line2[] = "HELLO: ./string \"abcd\"";
    AssemblyLine parsedLine1 = parseAssemblyLine(line1);
    AssemblyLine parsedLine2 = parseAssemblyLine(line2);
    printf("Parsed Line 1:\n");
    printAssemblyLine2(&parsedLine1);
    printf("\nParsed Line 2:\n");
    printAssemblyLine2(&parsedLine2);
    /*Free allocated memory*/
    freeAssemblyLine(parsedLine1);
    freeAssemblyLine(parsedLine2);
    return 0;

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