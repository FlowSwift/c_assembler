#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
#include "symbol_table.h"
#include "util.h"
#include "constant.h"
#include "error_handler.h"
#include "macros.h"
#include "binary_conversion.h"
#include "command_handler.h"
#include "directive_handler.h"

int first_pass(char *file_name, struct macros *macro_head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    char *am_file_name = NULL;
    AssemblyLine parsedLine; /*initializing an Assembly line structure*/
    SymbolNode *current_symbol = NULL;
    BinaryLine *instruction_binary_table = NULL;
    BinaryLine *directive_binary_table = NULL;
    BinaryLine *temp_instruction = NULL;
    BinaryLine *temp_directive = NULL;
    int line_number = 0;
    int is_symbol = 0;
    int temp_memory_place = 0;
    int was_error = 0;        /* -1 if error happened at all during first_pass, 0 otherwise */
    ErrorCode error_flag = 0; /*Assume success*/
    FILE *amfile = NULL;
    BinaryLine *current_instruction = NULL, *current_directive = NULL;

    am_file_name = add_file_extension(file_name, ".am");
    amfile = fopen(am_file_name, "r");
    free(am_file_name);
    if (amfile == NULL) /*if file opening failed*/
    {
        error_flag = ERROR_FILE_NOT_FOUND;
        return error_flag;
    }
    while (fgets(line, MAX_LINE_LENGTH, amfile) != NULL) /*goes through every line until the end of file*/
    {
        error_flag = 0;
        temp_memory_place = -1;
        line_number++;
        if ((is_empty_line(line) == 0) || (is_commented_line(line) == 0))
        { /*continue to next line if line is empty or commented.*/
            continue;
        }
        parsedLine = parseAssemblyLine(line, line_number); /* Parse line for this format: label{optional},instruction, operands.*/
        if (parsedLine.instruction == NULL)                /*parse line function failed*/
        {
            error_flag = ERROR_PARSE_LINE_FAILED;
            handle_error(ERROR_PARSE_LINE_FAILED, line_number);
        }
        if (parsedLine.label != NULL) /*a label is defined. NULL when not.*/
        {
            is_symbol = 1; /*update is_symbol flag, will add the label if the line is valid.*/
        }
        if (parsedLine.instruction[0] == '.') /* if '.' is found it is directive line.*/
        {
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0) /* if data*/
            {
                temp_memory_place = *DC;
                error_flag = handle_data_directive(&parsedLine, symbolTable, &directive_binary_table, line_number, DC);
                if (is_symbol && (error_flag == 0))                                                                                            /*if handeling was succesful and a label is defined*/
                {                                                                                                                              /*data was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_DATA, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 1: data*/
                }
            }
            else if (strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0) /* if string*/
            {
                temp_memory_place = *DC;
                error_flag = handle_string_directive(&parsedLine, symbolTable, &directive_binary_table, line_number, DC);
                if (is_symbol && (error_flag == 0))                                                                                              /*if handeling was succesful and a label is defined*/
                {                                                                                                                                /*string was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_STRING, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 2: string*/
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0) /* if extern*/
            {
                error_flag = handle_extern_directive(&parsedLine, symbolTable, &instruction_binary_table, macro_head);
                if (is_symbol)
                {                                                      /*extern was in correct format and has symbol definition*/
                    handle_error(ERROR_EXTERN_NOT_VALID, line_number); /*Extern directive cannot have a label definition.*/
                }
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0) /* if entry*/
            {
                error_flag = handle_entry_directive(&parsedLine, symbolTable, &instruction_binary_table, macro_head);
                if (is_symbol)
                {                                                     /*extern was in correct format and has symbol definition*/
                    handle_error(ERROR_ENTRY_NOT_VALID, line_number); /*Entry directive cannot have a label definition.*/
                }
            }
            else /*if the instruction starts with . and is not one of defined directives.*/
            {
                error_flag = ERROR_NOT_DEFINED_DIRECTIVE;
            }
        }
        else /*if the instrution doesn't start with '.' assumes the line is command.*/
        {
            temp_memory_place = *IC;
            error_flag = handle_instruction(&parsedLine, symbolTable, &instruction_binary_table, IC, macro_head, line_number); /*label is defined inside, also makes binary*/
            if (is_symbol && (error_flag == 0))                                                                                /*if handeling was succesful and a label is defined*/
            {
                /* has label and is in regular instruction format.*/
                error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_INSTRUCTION, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*also checks if name is legal, symbol gets IC place*/
            }
        }
        if (error_flag != 0) /*if handeling failed or the instruction wasn't allowed.*/
        {
            was_error = -1;
            handle_error(error_flag, line_number);
        }
        is_symbol = 0; /*return symbol flag to 0*/

        /* check if memory overflow, above the allowed max memory.*/
        if ((*IC + *DC) > MAX_MEMORY_WORDS)
        {
            error_flag = ERROR_MEMORY_OVERFLOW;
            handle_error(error_flag, line_number);
            return error_flag;
        }
        freeAssemblyLine(&parsedLine); /*after handeling, releases the AssemblyLine*/
    }

    /*Adds 100 for the IC for every instruction */
    current_instruction = instruction_binary_table, current_directive = directive_binary_table;
    while (current_instruction != NULL)
    {
        (current_instruction)->decimal_memory_address += 100;
        current_instruction = (current_instruction)->next;
    }

    /*Adds 100 and IC ending value for the DC for every directive(.data or .string).*/
    while (current_directive != NULL)
    {
        (current_directive)->decimal_memory_address += *IC + 100;
        current_directive = (current_directive)->next;
    }
    current_instruction = instruction_binary_table;
    current_directive = directive_binary_table;
    temp_instruction = (current_instruction);
    temp_directive = (current_directive);
    /*adds binary codes of instruction and directive lines, first command lines and then directive.*/
    add_binary_lines(temp_directive, &temp_instruction);
    *binary_table = temp_instruction;
    /*add 100 + IC for every symbol defining directive line and 100 for every symbol defining command line.*/
    current_symbol = symbolTable->head;
    while (current_symbol != NULL)
    {
        if ((current_symbol->type == TYPE_DATA) || (current_symbol->type == TYPE_STRING)) /* data type is {data}*/
        {
            current_symbol->memory_place += *IC + 100;
        }
        else if (current_symbol->type == TYPE_INSTRUCTION)
        {
            current_symbol->memory_place += 100;
        }
        current_symbol = current_symbol->next;
    }
    current_symbol = symbolTable->head;
    fclose(amfile);
    return was_error; /* 0 -> SUCCESS*/
}