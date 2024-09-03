#include <stdlib.h>
#include "preprocessor.h"
#include "first_pass.h"
#include "symbol_table.h"
#include "macros.h"
#include "binary_conversion.h"
#include "second_pass.h"
#include "util.h"

int main(int argc, char *argv[])
{
    struct macros *macro_head = NULL;
    SymbolTable *symbolTable = NULL;
    BinaryLine *binary_table = NULL;
    int IC, DC;
    int file_counter = 1;
    while (file_counter < argc)
    {
        free_all(&macro_head, &symbolTable, &binary_table);
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("File: %s\n", argv[file_counter]);
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        IC = 0, DC = 0;
        if (pre_process(argv[file_counter], &macro_head))
        {
            printf("Error in preprocessor for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        symbolTable = createSymbolTable(); /*create empty symbol table*/
        printf("FIRST PASS:\n");
        printf("FIRST PASS:\n");
        printf("FIRST PASS:\n");
        if (first_pass(argv[file_counter], macro_head, symbolTable, &binary_table, &IC, &DC))
        {
            printf("Error in first pass for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        printf("SECOND PASS:\n");
        printf("SECOND PASS:\n");
        printf("SECOND PASS:\n");
        if (second_pass(argv[file_counter] ,macro_head, symbolTable, &binary_table, &IC, &DC))
        {
            printf("Error in second pass for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        file_counter++;
    }
    printf("FINISHED FILES\n");
    free_all(&macro_head, &symbolTable, &binary_table);
    return 0;
}