#include <stdio.h>

#include "second_pass.h"
#include "symbol_table.h"
#include "binary_conversion.h"
#include "error_handler.h"

int second_pass(char *filename, struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC)
{
    BinaryLine *current_line = *binary_table;
    SymbolNode *current_symbol = symbolTable->head;
    ErrorCode error_flag = ERROR_NONE;
    while (current_line != NULL)
    {
        if (current_line->label != NULL)
        {
            current_symbol = is_symbol_in_table(symbolTable, current_line->label);
            if (current_symbol != NULL)
            {
                convert_label_adress_to_binary_line(current_line, current_symbol);
            }
            else
            {
                error_flag = ERROR_SYMBOL_WAS_NOT_DEFINED;
                printf("SYMBOL NAME: %s\n", current_line->label);
                handle_error(error_flag, current_line->original_line_number);
            }
        }
        current_line = current_line->next;
    }
    if (error_flag != ERROR_NONE)
    {
        return error_flag;
    }
    int i = 1;
    char bin[16];
    BinaryLine *temp_line = *binary_table;
    while (temp_line != NULL)
    {
        printf("Binary Line: %d\n", i);
        decimal_to_binary((temp_line)->binary_code, bin, 16);
        printf("Binary Line: %s\n", bin);
        printf("Label: %s\n", (temp_line)->label);
        printf("Binary code: %d\n", (temp_line)->binary_code);
        printf("Original line number: %d\n", (temp_line)->original_line_number);
        printf("Decimal memory address: %d\n", (temp_line)->decimal_memory_address);
        i++;
        printf("---------------------\n");
        temp_line = (temp_line)->next;
    }
    return error_flag;
}
