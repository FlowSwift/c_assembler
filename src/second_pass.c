#include <stdio.h>

#include "second_pass.h"
#include "symbol_table.h"
#include "binary_conversion.h"
#include "error_handler.h"

int second_pass(struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC)
{
    BinaryLine *current_line = *binary_table;
    SymbolNode *current_symbol = symbolTable->head;
    char octal_code[6];
    ErrorCode error_flag = ERROR_NONE;
    // printf("ALL SYMBOLS:\n");
    // while (current_symbol != NULL) {
    //     printf("Symbol: %s\n", current_symbol->name);
    // }
    // printf("END OF SYMBOLS\n");
    if (validate_symbols(symbolTable, binary_table))
    {
        return error_flag;
    }
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
                printf("Label: -%s-\n", current_line->label);
                error_flag = ERROR_SYMBOL_WAS_NOT_DEFINED;
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
    FILE *file = fopen("output.ob", "w");
    if (file == NULL)
    {
        error_flag = ERROR_CANT_WRITE_FILE;
        return error_flag;
    }
    temp_line = *binary_table;
    fprintf(file, "%d %d\n", *IC, *DC);
    while (temp_line != NULL)
    {
        decimal_to_octal(temp_line->binary_code, octal_code, 6);
        fprintf(file, "%04d %s\n", temp_line->decimal_memory_address, octal_code);
        temp_line = temp_line->next;
    }
    fclose(file);
    return error_flag;
}

int validate_symbols(SymbolTable *symbolTable, BinaryLine **binary_table)
{
    BinaryLine *current_line = *binary_table;
    SymbolNode *current_symbol = symbolTable->head;
    ErrorCode error_flag = ERROR_NONE;
    while (current_symbol != NULL)
    {
        if ((current_symbol->label_type == TYPE_ENTRY) && (current_symbol->memory_place == 0))
        {
            printf("An error occured for the following symbol: %s\n", current_symbol->name);
            error_flag = ERROR_ENTRY_NOT_VALID;
            handle_error(error_flag, 0);
        }
        else if((current_symbol->label_type == TYPE_EXTERN) && (current_symbol->memory_place != 0))
        {
            printf("An error occured for the following symbol: %s\n", current_symbol->name);
            error_flag = ERROR_EXTERN_NOT_VALID;
            handle_error(error_flag, 0);
        }
        current_symbol = current_symbol->next;
    }
    return error_flag;
}