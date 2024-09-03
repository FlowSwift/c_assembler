#include <stdio.h>
#include <stdlib.h>

#include "second_pass.h"
#include "symbol_table.h"
#include "binary_conversion.h"
#include "error_handler.h"
#include "util.h"

int second_pass(char *file_name, struct macros *head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC)
{
    BinaryLine *current_line = *binary_table;
    SymbolNode *current_symbol = symbolTable->head;
    char octal_code[6];
    ErrorCode error_flag = ERROR_NONE;
    char *ob_file_name = NULL;
    // printf("ALL SYMBOLS:\n");
    // while (current_symbol != NULL) {
    //     printf("Symbol: %s\n", current_symbol->name);
    // }
    // printf("END OF SYMBOLS\n");
    if (validate_symbols(symbolTable))
    {
        return -1;
    }
    if (create_ent_file(file_name, symbolTable))
    {
        return -1;
    }
    if (create_ext_file(file_name, symbolTable, binary_table))
    {
        return -1;
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
    ob_file_name = add_file_extension(file_name, ".ob");
    FILE *file = fopen(ob_file_name, "w");
    free(ob_file_name);
    if (file == NULL)
    {
        error_flag = ERROR_CANT_WRITE_FILE;
        return error_flag;
    }
    temp_line = *binary_table;
    fprintf(file, "   %d %d\n", *IC, *DC);
    while (temp_line != NULL)
    {
        decimal_to_octal(temp_line->binary_code, octal_code, 6);
        fprintf(file, "%04d %s\n", temp_line->decimal_memory_address, octal_code);
        temp_line = temp_line->next;
    }
    fclose(file);
    return error_flag;
}

int validate_symbols(SymbolTable *symbolTable)
{
    SymbolNode *current_symbol = symbolTable->head;
    ErrorCode error_flag = ERROR_NONE;
    while (current_symbol != NULL)
    {
        if ((current_symbol->label_type == TYPE_ENTRY) && (current_symbol->memory_place == 0))
        {
            printf("An error occured for the following symbol: \"%s\". - ", current_symbol->name);
            error_flag = ERROR_ENTRY_WAS_NOT_DEFINED;
            handle_error(error_flag, 0);
        }
        else if ((current_symbol->label_type == TYPE_EXTERN) && (current_symbol->memory_place != 0))
        {
            printf("An error occured for the following symbol: \"%s\". - ", current_symbol->name);
            error_flag = ERROR_EXTERN_WAS_DEFINED;
            handle_error(error_flag, 0);
        }
        current_symbol = current_symbol->next;
    }
    return error_flag;
}

int create_ent_file(char *file_name, SymbolTable *symbolTable)
{
    SymbolNode *current_symbol = symbolTable->head;
    FILE *ent_file = NULL; /* Only create file if a symbol was found */
    ErrorCode error_code = ERROR_NONE;
    char *ent_file_name = NULL;
    while (current_symbol != NULL)
    {
        if (current_symbol->label_type == TYPE_ENTRY)
        {
            if (ent_file == NULL)
            {
                ent_file_name = add_file_extension(file_name, ".ent");
                ent_file = fopen(ent_file_name, "w");
                free(ent_file_name);
                if (ent_file == NULL)
                {
                    error_code = ERROR_CANT_WRITE_FILE;
                    handle_error(error_code, 0);
                    return error_code;
                }
            }
            fprintf(ent_file, "%s %d\n", current_symbol->name, current_symbol->memory_place);
        }
        current_symbol = current_symbol->next;
    }
    if (ent_file != NULL)
        fclose(ent_file);
    return error_code;
}

int create_ext_file(char *file_name, SymbolTable *symbolTable, BinaryLine **binary_table)
{
    SymbolNode *current_symbol = symbolTable->head;
    BinaryLine *current_binary_line = *binary_table;
    FILE *ext_file = NULL;
    ErrorCode error_code = ERROR_NONE;
    char *ext_file_name = NULL;
    while (current_binary_line != NULL)
    {
        if (current_binary_line->label != NULL)
        {
            current_symbol = is_symbol_in_table(symbolTable, current_binary_line->label);
            if (current_symbol != NULL)
            {
                if (current_symbol->label_type == TYPE_EXTERN)
                {
                    if (ext_file == NULL)
                    {
                        ext_file_name = add_file_extension(file_name, ".ext");
                        ext_file = fopen(ext_file_name, "w");
                        free(ext_file_name);
                        if (ext_file == NULL)
                        {
                            error_code = ERROR_CANT_WRITE_FILE;
                            handle_error(error_code, 0);
                            return error_code;
                        }
                    }
                    fprintf(ext_file, "%s %04d\n", current_binary_line->label, current_binary_line->decimal_memory_address);
                }
            }
        }
        current_binary_line = current_binary_line->next;
    }
    if (ext_file != NULL)
        fclose(ext_file);
    return error_code;
}