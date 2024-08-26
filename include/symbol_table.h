#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

typedef enum
{
    TYPE_LABEL_DEF = 0,
    TYPE_INSTRUCTION,
    TYPE_DATA,
    TYPE_STRING,
    TYPE_ENTRY,
    TYPE_EXTERN
} SymbolType;

/*
    Todo:
        Modify to use only SymbolNode without SymbolTable
*/
#include "macros.h"
typedef struct SymbolNode
{
    /*every label will have this structure after it is checked that it is legal definition */
    char *name;
    int type; /* Instruction, data or string */
    int label_type; /* Label def, entry or extern */
    int memory_place;
    struct SymbolNode *next;
} SymbolNode;

typedef struct SymbolTable
{
    /*linked list of all the symbols*/
    SymbolNode *head;
    SymbolNode *last;
} SymbolTable;

/*searchs if symbol is in table*/
SymbolNode *is_symbol_in_table(SymbolTable *table, char *symbol_name);
/*cheks if symbol name is legal*/
int is_valid_symbol(struct macros *head, char *label);
/*adds symbol to table - cheks inside if symbol name is legal(is_valid_symbol)*/
int add_symbol_to_table(SymbolTable *table, char *symbol_name, int type, int label_type, int memory_place, struct macros *macro_head);
/*creates empty symbol table*/
SymbolTable *createSymbolTable();
/*frees symbol Node in Table*/
void freeSymbolNode(SymbolNode *node);
/*frees symbol table*/
void freeSymbolTable(SymbolTable *table);

#endif /* __SYMBOL_TABLE_H__ */