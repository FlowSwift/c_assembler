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
    SymbolNode *last;
} SymbolTable;

typedef struct Operand{
    /*every operand will have this structure */
    char name;
    int type; /*miun types */
    int memory_place;
    int is_label;
} Operand;

typedef struct AssemblyLine{
    /*every line will be parsed and inputed to this structure */
    char *label; /*will be NULL at first*/
    char *instruction;
    char *operands;
    Operand *srcOperand;
    Operand *destOperand;
}AssemblyLine;

AssemblyLine parseAssemblyLine(const char *line)
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
    return parsedLine;
}
void PrintAssemblyLine(const AssemblyLine *line) {
    printf("Label: %s\n", line->label ? line->label : "NULL");
    printf("Instruction: %s\n", line->instruction ? line->instruction : "NULL");
    printf("Operands: %s\n", line->operands ? line->operands : "NULL");
}
void printAss(AssemblyLine line){
	printf("Label: %s\n", line.label ? line.label : "NULL");
    printf("Instruction: %s\n", line.instruction ? line.instruction : "NULL");
    printf("Operands: %s\n", line.operands ? line.operands : "NULL");
}

void printAssemblyLine2(AssemblyLine *parsedLine) {
    if (parsedLine == NULL) {
        printf("AssemblyLine is NULL");
    }
    else {
        printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
        printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
        printf("Operands: %s\n", parsedLine->operands);
    }
}

int main(){
	char line = "MAIN: add r3, LIST";
	AssemblyLine parsedLine = parseAssemblyLine(&line);
	AssemblyLine* p_parsed = &parsedLine;
	printAssemblyLine2(&p_parsed);
}
