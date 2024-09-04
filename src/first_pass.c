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
    char bin[16];
    AssemblyLine parsedLine;
    SymbolNode *current_symbol = NULL;
    BinaryLine *instruction_binary_table = NULL;
    BinaryLine *directive_binary_table = NULL;
    BinaryLine *temp_instruction = NULL;
    BinaryLine *temp_directive = NULL;
    int line_number = 0;
    int is_symbol = 0;
    int temp_memory_place = 0;
    int i = 0;
    ErrorCode error_flag = 0; /*assume success*/
    FILE *amfile = NULL;
    BinaryLine *current_instruction = NULL, *current_directive = NULL;
    am_file_name = add_file_extension(file_name, ".am");
    amfile = fopen(am_file_name, "r");
    free(am_file_name);
    if (amfile == NULL)
    {
        error_flag = ERROR_FILE_NOT_FOUND;
        return error_flag;
    }
    while (fgets(line, MAX_LINE_LENGTH, amfile) != NULL)
    {
        printf("----------------\n");
        error_flag = 0;
        temp_memory_place = 0;
        line_number++;
        if ((is_empty_line(line) == 0) || (is_commented_line(line) == 0))
        { /*continue to next line if line is empty or commented.*/
            continue;
        }
        parsedLine = parseAssemblyLine(line); /* Parse line for this format: label{optional},instruction, operands.*/
        printf("Parsed Line %d:\n", line_number);
        printAssemblyLine(&parsedLine);
        if (parsedLine.label != NULL)
        {                  /*a label is defined. NULL when not.*/
            is_symbol = 1; /*update is_sunbol flag*/
        }
        if (parsedLine.instruction[0] == '.')
        { /* if '.' is found it is directive line.*/
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0)
            { /* if data*/
                temp_memory_place = *DC;
                error_flag = handle_data_directive(&parsedLine, symbolTable, &directive_binary_table, line_number, DC);
                if (is_symbol && (error_flag == 0))
                {                                                                                                                              /*data was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_DATA, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 1: data*/
                }
            }
            else if (strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0)
            { /* if string*/
                temp_memory_place = *DC;
                error_flag = handle_string_directive(&parsedLine, symbolTable, &directive_binary_table, line_number, DC);
                if (is_symbol && (error_flag == 0))
                {                                                                                                                                /*string was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_STRING, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 2: string*/
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0)
            { /* if extern*/
                error_flag = handle_extern_directive(&parsedLine, symbolTable, &instruction_binary_table, macro_head);
                if (is_symbol)
                {                                                                                                                                     /*extern was in correct format and has symbol definition*/
                    handle_error(ERROR_EXTERN_NOT_VALID, line_number); /*Extern directive cannot have a label*/
                }
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0)
            { /* if entry*/
                error_flag = handle_entry_directive(&parsedLine, symbolTable, &instruction_binary_table, macro_head);
                if (is_symbol)
                {                                                                                                                                     /*extern was in correct format and has symbol definition*/
                    handle_error(ERROR_ENTRY_NOT_VALID, line_number); /*Extern directive cannot have a label*/
                }
            }
            else
            { /*if not one of defined directives.*/
                error_flag = ERROR_NOT_DEFINED_DIRECTIVE;
            }
        }
        else
        { /* assume line is command - maybe also label is defined*/
            temp_memory_place = *IC;
            error_flag = handle_instruction(&parsedLine, symbolTable, &instruction_binary_table, IC, macro_head, line_number); /*label is defined inside, also makes binary*/
            if (is_symbol && (error_flag == 0))
            {                                                                                                                                     /* has label and is in regular instruction format.*/
                error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_INSTRUCTION, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*also checks if name is legal, symbol gets IC place*/
            }
        }
        if (error_flag != 0)
        { /*if something failed*/
            handle_error(error_flag, line_number);
        }
        is_symbol = 0; /*return symbol flag to 0*/
        /* check if memory overflow*/
        printf("IC: %d\n", *IC);
        printf("DC: %d\n", *DC);
        if ((*IC + *DC) > MAX_MEMORY_WORDS)
        {
            error_flag = ERROR_MEMORY_OVERFLOW;
            handle_error(error_flag, line_number);
            return error_flag;
        }
        if (get_opcode_code(parsedLine.instruction) != -1)
        {
            if (parsedLine.srcOperand != NULL)
            {
                printf("Source Operand: %s\n", parsedLine.srcOperand->value);
                printf("Type of src %d\n", parsedLine.srcOperand->type);
            }
            if (parsedLine.destOperand != NULL)
            {
                printf("Destination Operand: %s\n", parsedLine.destOperand->value);
                printf("Type of dest %d\n", parsedLine.destOperand->type);
            }
        }
        freeAssemblyLine(&parsedLine);
    }
    current_instruction = instruction_binary_table, current_directive = directive_binary_table;
    while (current_instruction != NULL)
    {
        (current_instruction)->decimal_memory_address += 100;
        current_instruction = (current_instruction)->next;
    }
    while (current_directive != NULL)
    {
        (current_directive)->decimal_memory_address += *IC + 100;
        current_directive = (current_directive)->next;
    }
    current_instruction = instruction_binary_table;
    current_directive = directive_binary_table;
    i = 1;
    temp_instruction = (current_instruction);
    temp_directive = (current_directive);
    while (current_instruction != NULL)
    {
        printf("Binary Line: %d\n", i);
        decimal_to_binary((current_instruction)->binary_code, bin, 16);
        printf("Binary Line: %s\n", bin);
        printf("Label: %s\n", (current_instruction)->label);
        printf("Binary code: %d\n", (current_instruction)->binary_code);
        printf("Original line number: %d\n", (current_instruction)->original_line_number);
        printf("Decimal memory address: %d\n", (current_instruction)->decimal_memory_address);
        i++;
        printf("---------------------\n");
        current_instruction = (current_instruction)->next;
    }
    i = 1;
    printf("DIRECTIVE: \n");
    while (current_directive != NULL)
    {
        printf("Binary Line: %d\n", i);
        decimal_to_binary((current_directive)->binary_code, bin, 16);
        printf("Binary Line: %s\n", bin);
        printf("Label: %s\n", (current_directive)->label);
        printf("Binary code: %d\n", (current_directive)->binary_code);
        printf("Original line number: %d\n", (current_directive)->original_line_number);
        printf("Decimal memory address: %d\n", (current_directive)->decimal_memory_address);
        i++;
        printf("---------------------\n");
        current_directive = (current_directive)->next;
    }
    i = 0;
    add_binary_lines(temp_directive, &temp_instruction);
    *binary_table = temp_instruction;
    while (temp_instruction != NULL)
    {
        decimal_to_binary(temp_instruction->binary_code, bin, 16);
        printf("%d: %s\n", i, bin);
        i++;
        temp_instruction = temp_instruction->next;
    }
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
    printf("-------------------\n");
    printf("SYMBOL TABLE:\n");
    while (current_symbol != NULL)
    {
        printf("----------------\n");
        printf("Symbol: -%s-\n", current_symbol->name);
        printf("Type: %d\n", current_symbol->type);
        printf("Label type: %d\n", current_symbol->label_type);
        printf("Memory place: %d\n", current_symbol->memory_place);
        current_symbol = current_symbol->next;
    }
    printf("-------------------\n");
    fclose(amfile);
    return error_flag; /* 0 -> SUCCESS*/
}