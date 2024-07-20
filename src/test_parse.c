#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct SymbolNode{
    /*every label will have this structure after it is checked that it is legal definition */
    char name;
    int type; /*entry, extern, new defenition*/
    int *memory_place;
}SymbolNode;

typedef struct SymbolTable{
    /*linked list of all the symbols*/
    SymbolNode *head;
	SymbolNode *tail;
}SymbolTable;

typedef struct Operand {
    char *type;
    char *value;
} Operand;

typedef struct AssemblyLine {
    char *label;
    char *instruction;
    char *operands;
    Operand *srcOperand;
    Operand *destOperand;
} AssemblyLine;

AssemblyLine parseAssemblyLine(const char *line) {
    AssemblyLine parsedLine;
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

        /* Move past space to  operands*/
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
    free(parsedLine1.label);
    free(parsedLine1.instruction);
    free(parsedLine1.operands);

    free(parsedLine2.label);
    free(parsedLine2.instruction);
    free(parsedLine2.operands);

    return 0;
}
