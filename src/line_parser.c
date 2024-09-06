#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "line_parser.h"
#include "util.h"
#include "error_handler.h"

AssemblyLine parseAssemblyLine(char *line, int line_number)
{
    AssemblyLine parsedLine;
    int line_length = strlen(line);
    char *space_position = NULL;
    char *colon_position = NULL;
    /*Initialize parsed Line structure */
    parsedLine.label = NULL;
    parsedLine.instruction = NULL;
    parsedLine.operands = NULL;
    parsedLine.srcOperand = NULL;
    parsedLine.destOperand = NULL;
    if (line[(line_length - 1)] == '\n')
    {
        line[strlen(line) - 1] = '\0'; /*Remove the newline character*/
    }
    /* Find the position of the colon*/
    colon_position = strchr(line, ':');
    if ((colon_position != NULL) && ((*(colon_position + 1)) == ' ')) /*must be a whitespace after ":"*/
    {
        /* Extract and set the label*/
        size_t labelLen = colon_position - line;
        parsedLine.label = (char *)malloc(labelLen + 1);
        if (parsedLine.label == NULL) /*check malloc*/
        {
            handle_error(ERROR_MEMORY_ALLOCATION_FAILED, line_number);
            parsedLine.instruction = NULL; /*return this value because it is always in line and label is optional. if failed -> NULL*/
            return parsedLine;
        }
        strncpy(parsedLine.label, line, labelLen);
        parsedLine.label[labelLen] = '\0';
        /* Move past the colon*/
        line = colon_position + 1;
        while (*line == ' ')
        {
            line++;
        }
    }
    /*Find the position of the first space after the label (if any)*/
    space_position = strchr(line, ' ');
    if (space_position != NULL)
    {
        /* Extract and set the instruction*/
        size_t instrLen = space_position - line;
        parsedLine.instruction = (char *)malloc(instrLen + 1);
        if (parsedLine.instruction == NULL)
        {
            handle_error(ERROR_MEMORY_ALLOCATION_FAILED, line_number);
            parsedLine.instruction = NULL;
            return parsedLine;
        }
        strncpy(parsedLine.instruction, line, instrLen);
        parsedLine.instruction[instrLen] = '\0';
        /* Move past space to operands*/
        line = space_position + 1;
        /* Set operands*/
        if (*line != '\0')
        {
            parsedLine.operands = strdup1(line); /*operands are set by what is after the space. will be put in SrcOperand and DestOperand later*/
        }
        else
        {
            parsedLine.operands = NULL;
        }
    }
    else
    {
        /*If there is no space, the entire line is the instruction with no operands*/
        parsedLine.instruction = strdup1(line);
    }
    return parsedLine;
}
void printAssemblyLine(const AssemblyLine *parsedLine)
{
    if (parsedLine == NULL)
    {
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
        if (line->label != NULL)
        {
            free(line->label);
        }
        if (line->instruction != NULL)
        {
            free(line->instruction);
        }
        if (line->operands != NULL)
        {
            free(line->operands);
        }
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
        if (operand->value != NULL)
        {
            free(operand->value);
        }
        free(operand);
    }
}