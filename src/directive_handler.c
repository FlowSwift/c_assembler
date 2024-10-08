#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive_handler.h"
#include "line_parser.h"
#include "symbol_table.h"
#include "error_handler.h"
#include "binary_conversion.h"
#include "constant.h"
#include "util.h"

int handle_string_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line_number, int *DC)
{
    int stringLen = 0, i = 0;
    char binaryCode[BINARY_CODE_LEN];
    BinaryLine *binary_line = NULL, *head = NULL;
    ErrorCode error_flag = 0; /*Assume success*/
    stringLen = strlen(parsedLine->operands);
    memset(binaryCode, '\0', sizeof(binaryCode));

    error_flag = is_valid_string(parsedLine->operands); /*doesnt start and end with quotation "".*/
    if (error_flag != 0)
    { 
        return error_flag;
    }

    /*go through string without ""*/
    for (i = 1; i < stringLen - 1; i++)
    {
        /* converts the ASCII value of the character to a binary string */
        if (binary_line == NULL) /*if line is empty*/
        {
            binary_line = convert_directive_to_binary_code(parsedLine->operands[i], line_number, *DC);
            head = binary_line;
        }
        else
        {
            binary_line->next = convert_directive_to_binary_code(parsedLine->operands[i], line_number, *DC);
            binary_line = binary_line->next;
        }
        *DC = *DC + 1;
    }
    if (binary_line == NULL)
    {
        binary_line = convert_directive_to_binary_code(0, line_number, *DC); /* add null terminator */
        head = binary_line;
    }
    else
    {
        binary_line->next = convert_directive_to_binary_code(0, line_number, *DC); /* add null terminator */
    }
    add_binary_lines(head, directive_binary_table);
    *DC = *DC + 1;     /*for '/0' at the end*/
    return error_flag; /* 0 -> SUCCESS*/
}

int handle_data_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line_number, int *DC)
{
    char *token = NULL;
    int value = 0;
    ErrorCode error_flag = 0; /*Assume success*/
    BinaryLine *binary_line = NULL, *head = NULL;
    if (parsedLine->operands[strlen(parsedLine->operands) - 1] == ',')
    {
        return ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
    }
    token = strtok(parsedLine->operands, ","); /*split operands by ','*/
    while (token != NULL)
    {
        if (strlen(token) == 0)
        {
            error_flag = ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
            return error_flag;
        }
        token = trim_whitespace(token); /*Trim leading and ending whitespace - use this string for next checks*/
        error_flag = is_valid_integer(token);
        if (error_flag != 0)
        {
            error_flag = ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
            return error_flag;
        }
        if (error_flag == 0) {/*operand is integer*/
            value = atoi(token); /*numeral value of the operand*/
            if (value > MAX_DATA_INT)
            {
                error_flag = ERROR_INTEGER_VALUE_TOO_BIG; /*integer value too big*/
            }
            else if (value < MIN_DATA_INT)
            {
                error_flag = ERROR_INTEGER_VALUE_TOO_SMALL; /*integer value too small*/
            }
        }
        if(error_flag != 0){ /*if value too big or small, return error.*/
            return error_flag;
        }

        token = strtok(NULL, ",");
        /* Insert the binary */
        if (binary_line == NULL) /*this is the first word in this binary line*/
        {
            binary_line = convert_directive_to_binary_code(value, line_number, *DC);
            head = binary_line;
        }
        else
        {
            binary_line->next = convert_directive_to_binary_code(value, line_number, *DC);
            binary_line = binary_line->next;
        }
        if (error_flag != 0)
        {
            return error_flag; /* some error in binary conversion*/
        }
        *DC = *DC + 1;
    }
    add_binary_lines(head, directive_binary_table);
    return error_flag; /* 0 -> SUCCESS*/
}


int handle_extern_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head)
{
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    token = strtok(parsedLine->operands, ","); /*in .extern the labels will be the operands.*/
    while (token != NULL) /*if number of labels is bigger then one*/
    {
        token = trim_whitespace(token); /*returns char pointer after whitespaces and ends before whitespaces.*/
        if (strlen(token) == 0)
        { /*symbol empty, can happen between two ','*/
            error_flag = ERROR_SYMBOL_SHORT;
            return error_flag;
        }
        error_flag = add_symbol_to_table(symbolTable, token, -1, TYPE_EXTERN, -1, macro_head); /*extern is type 4. checks inside if the symbol is in valid name*/
        if (error_flag != 0) /*if some label not valid.*/
        {
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}

int handle_entry_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head)
{
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    token = strtok(parsedLine->operands, ","); /*in .entry the labels will be the operands.*/
    while (token != NULL)
    {
        token = trim_whitespace(token); /*returns char pointer after whitespaces and ends before whitespaces.*/
        if (strlen(token) == 0)
        { /*symbol empty, can happen between two ','*/
            error_flag = ERROR_SYMBOL_SHORT;
            return error_flag;
        }
        error_flag = add_symbol_to_table(symbolTable, token, -1, TYPE_ENTRY, -1, macro_head); /*extern is type 3. checks inside if the symbol is in valid name*/
        if (error_flag != 0) /*if some label not valid.*/
        { 
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}