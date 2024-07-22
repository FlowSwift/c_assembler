
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "constant.h"
#include "first_pass.h"
#include "error_handler.h"
#include "macros.h"

int is_symbol_in_table(SymbolTable *table, char *symbol_name) {
    SymbolNode *current = NULL;
    current = table->head;
    while (current != NULL) {
        if (strcmp(current->name, symbol_name) == 0) {
            return 0; /* Symbol found*/
        }
        current = current->next;
    }
    return 1; /*Symbol not found*/
}

int is_valid_symbol(struct macros *macro_head, char* label)
{
    int i = 1;
    int j = 0;
    ErrorCode error_flag = 0; /*assume success*/
    if (strlen(label) > MAX_LABEL_LEN)
    {
        error_flag = ERROR_SYMBOL_TOO_LONG;
        return ERROR_SYMBOL_TOO_LONG;
    }
    /* Check if the first character is an alphabetic letter */
    if (!isalpha(label[0]))
    {
        error_flag = ERROR_SYMBOL_SYNTAX_IS_WRONG;
        return error_flag;
    }
    /* Check the rest of the characters */
    for (i = 1; i < strlen(label); i++)
    {
        if (!isalnum(label[i]))
        {
            error_flag = ERROR_SYMBOL_SYNTAX_IS_WRONG;
            return error_flag;
        }
    }
    /*check if label is opcode*/
    for (j = 0; j < strlen(label); j++){
        if (strcmp(label, OPCODES[j].name) == 0) {
            error_flag = ERROR_SYMBOL_NAME_IS_OPCODE;
            return error_flag;
        }
    }
    /*check if label is register name*/
    if(valid_reg_name(label)){
        error_flag = ERROR_SYMBOL_NAME_IS_REGISTER;
        return error_flag;
    }
    /*check if label is macro name*/
    if(is_existing_macro(macro_head, label) != NULL){ /*found label in macro table*/
        error_flag = ERROR_SYMBOL_NAME_IS_MACRO;
        return error_flag;
    }
    return error_flag; /* 0 if success*/
}

int add_symbol_to_table(SymbolTable *table, char *symbol_name, int symbol_type, int memory_place, struct macros *macro_head) {
    SymbolNode *new_node = NULL;
    ErrorCode error_flag = 0; /*assume success*/
    if (is_symbol_in_table(table, symbol_name) == 0) { /*symbol found in table*/
        error_flag = ERROR__SYMBOL_DEFINED_TWICE;
        return error_flag; /*Symbol already exists*/
    }
    /* Create a new node */ 
    new_node = (SymbolNode *)malloc(sizeof(SymbolNode));
    if (!new_node) {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
    }
    if((symbol_type == 1 || symbol_type == 2) && (is_valid_symbol(macro_head, symbol_name)!=0)){ /*check label definition only for DATA and STRING. for entry and extern label is not defined.*/
        error_flag = ERROR_SYMBOL_SYNTAX_IS_WRONG; /* checks if symbol is in correct format if data or string*/
        return error_flag;
    }
    new_node->name = strdup(symbol_name);
    new_node->type = symbol_type;
    new_node->memory_place = memory_place;
    if (!new_node->name || !new_node->type || !new_node->memory_place) {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        freeSymbolNode(new_node);
        return error_flag;
    }
    new_node->memory_place = memory_place;
    new_node->next = NULL;

    if (table->head == NULL) {
        table->head = new_node;
        table->last = new_node;
    } else if (table->last != NULL)
    {
        table->last->next = new_node;
    }
    table->last = new_node;
    return error_flag; /*if 0, Symbol added successfully*/
}

SymbolTable *createSymbolTable()
{
    SymbolTable *table = NULL;
    table = (SymbolTable *)malloc(sizeof(SymbolTable));
    ErrorCode error_flag = 0; /*assume success*/
    if (table == NULL)
    {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
    }
    table->head = NULL;
    table->last = NULL;
    return table;
}

void freeSymbolNode(SymbolNode *node) {
    if (node != NULL)
    {
        free(node->name);
        free(node);
    }
}

void freeSymbolTable(SymbolTable *table) {
    if (table != NULL)
    {
        SymbolNode *current = table->head;
        SymbolNode *tempNodeToFree;
        while (current != NULL)
        {
            tempNodeToFree = current;
            current = current->next;
            freeSymbolNode(tempNodeToFree);
        }
        table->head = NULL;
        table->last = NULL;
    }
}