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
        printf("----------------\n");
        printf("Starting file %s\n", argv[file_counter]);
        free_all(&macro_head, &symbolTable, &binary_table);
        IC = 0, DC = 0;
        printf("Starting pre_process for %s:\n", argv[file_counter]);
        if (pre_process(argv[file_counter], &macro_head))
        {
            printf("Error in preprocessor for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        printf("Done!\n");
        symbolTable = createSymbolTable(); /*create empty symbol table*/
        printf("Starting first_pass for %s:\n", argv[file_counter]);
        if (first_pass(argv[file_counter], macro_head, symbolTable, &binary_table, &IC, &DC))
        {
            printf("Error in first pass for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        printf("Done!\n");
        printf("Starting second_pass for %s:\n", argv[file_counter]);
        if (second_pass(argv[file_counter], macro_head, symbolTable, &binary_table, &IC, &DC))
        {
            printf("Error in second pass for the following file: %s\nWill continue to next file if available\n", argv[file_counter]);
            file_counter++;
            continue;
        }
        printf("Done!\n");
        printf("----------------\n");
        file_counter++;
    }
    printf("Finished all files!\n");
    free_all(&macro_head, &symbolTable, &binary_table);
    return 0;
}