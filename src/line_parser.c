#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "line_parser.h"

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
void printAssemblyLine(const AssemblyLine *parsedLine) {
        if (parsedLine == NULL) {
            printf("AssemblyLine is NULL\n");
        return;
    }
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands ? parsedLine->operands : "(none)");
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
            freeOperand(line->srcOperand);
        }
        if (line->destOperand != NULL)
        {
            freeOperand(line->destOperand);
        }
        line = NULL;
    }
}

void freeOperand(Operand *operand)
{
    if (operand != NULL)
    {
        free(operand->value);
        free(operand->memory_place);
        free(operand->is_label);
        free(operand);
        operand = NULL;
    }
}