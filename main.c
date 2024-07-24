#include "preprocessor.h"
#include "first_pass.h"

int main(int argc, char *argv[])
{
    struct macros *macro_head = NULL;   
    pre_process(argv[1], macro_head);
    SymbolTable *symbolTable = NULL;
    BinaryTable *binaryTable = NULL;
    symbolTable = createSymbolTable(); /*create empty symbol table*/
    binaryTable = create_binary_table(); /*create empty binary table*/
    int IC = 0;
    int DC = 0;
    printf("works 1");
    firstPass("test_firstpass",macro_head, symbolTable,binaryTable, &IC, &DC);
    printf("works 2");
    return 0;
}