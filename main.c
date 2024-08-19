#include <stdlib.h>
#include "preprocessor.h"
#include "first_pass.h"
#include "symbol_table.h"
#include "macros.h"
#include "binary_conversion.h"
#include "second_pass.h"

int main(int argc, char *argv[])
{
    struct macros *macro_head = NULL;
    pre_process("test_firstpass", macro_head);
    SymbolTable *symbolTable = NULL;
    BinaryLine *binary_table = NULL;
    symbolTable = createSymbolTable(); /*create empty symbol table*/
    int IC = 0;
    int DC = 0;
    printf("FIRST PASS:\n");
    printf("FIRST PASS:\n");
    printf("FIRST PASS:\n");
    first_pass("test_firstpass.am", macro_head, symbolTable, &binary_table, &IC, &DC);
    printf("SECOND PASS:\n");
    printf("SECOND PASS:\n");
    printf("SECOND PASS:\n");
    second_pass(macro_head, symbolTable, &binary_table, &IC, &DC);
    free_BinaryLine(binary_table);
    freeSymbolTable(symbolTable);
    return 0;
}