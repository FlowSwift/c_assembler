#include "preprocessor.h"
#include "first_pass.h"

int main(int argc, char *argv[])
{
    
    /* pre_process(argv[1]); */
    SymbolTable *symbolTable = NULL;
    symbolTable = createSymbolTable(); /*create empty symbol table*/
    BinaryTable = createBinaryTable(); /*create empty binary table*/
    int IC = 0;
    int DC = 0;
    firstPass("test_firstpass", symbolTable, &IC, &DC);
    return 0;
}