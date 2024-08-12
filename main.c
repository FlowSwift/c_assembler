#include <stdlib.h>
#include "preprocessor.h"
#include "first_pass.h"
#include "symbol_table.h"
#include "binary_table.h"
#include "macros.h"
#include "binary_conversion.h"

int main(int argc, char *argv[])
{
    struct macros *macro_head = NULL;
    /*pre_process(argv[1], macro_head);*/
    SymbolTable *symbolTable = NULL;
    struct BinaryLine *instruction_binary_table = NULL;
    struct BinaryLine *directive_binary_table = NULL;
    symbolTable = createSymbolTable(); /*create empty symbol table*/
    int IC = 0;
    int DC = 0;
    firstPass("test_firstpass", macro_head, symbolTable, &instruction_binary_table, &directive_binary_table, &IC, &DC);
    freeSymbolTable(symbolTable);
    return 0;
}