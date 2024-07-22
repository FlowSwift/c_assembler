#include "preprocessor.h"
#include "first_pass.h"

int main(int argc, char *argv[])
{
    
    /* pre_process(argv[1]); */
    SymbolTable *symbolTable = NULL;
    BinaryTable *binaryTable = NULL;
    symbolTable = createSymbolTable(); /*create empty symbol table*/
    binaryTable = create_binary_table(); /*create empty binary table*/
    int IC = 0;
    int DC = 0;
    firstPass("test_firstpass", symbolTable,binaryTable, &IC, &DC);
    return 0;
}