
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

int is_symbol_in_table(SymbolTable *table, char *symbol_name) {
    SymbolNode *current = table->head;
    while (current != NULL) {
        if (strcmp(current->name, symbol_name) == 0) {
            return 0; /* Symbol found*/
        }
        current = current->next;
    }
    return 1; /*Symbol not found*/
}

int is_valid_symbol(char* label)
{
    int i = 1;
    int j = 0;
    if (strlen(label) > MAX_LABEL_LEN)
    {
        /* IMRPOVE ERRORS */
        /* label too long*/
        return 0;
    }
    /* Check if the first character is an alphabetic letter */
    if (!isalpha(label[0]))
    {
        return 0;
    }
    /* Check the rest of the characters */
    for (i = 1; i < strlen(label); i++)
    {
        if (!isalnum(label[i]))
        {
            return 0;
        }
    }
    /*check if label is opcode*/
    for (j = 0; i < strlen(label); i++){
        if (strcmp(plabel, OPCODES[i].name) == 0) {
            return 0;
            /* IMRPOVE ERRORS*/
            /*label had instruction name*/
        }
    }
    /*check if label is register name*/
    if(valid_reg_name(label)){
        return 0;
        /* IMRPOVE ERRORS*/
    }
    /* ADD CHECK IF IN MACRO NAME */
    return 1;
}

int add_symbol_to_table(SymbolTable *table, char *symbol_name, int symbol_type, int memory_place) {
    SymbolNode *new_node = NULL;
    if (is_symbol_in_table(table, symbol_name)) {
        /* IMRPOVE ERRORS*/
        return 0; /*Symbol already exists*/
    }
    /* Create a new node */ 
    SymbolNode *new_node = (SymbolNode *)malloc(sizeof(SymbolNode));
    if (!new_node) {
        /* IMRPOVE ERRORS*/
        return 0; /*Memory allocation failed*/
    }
    if((symbol_type == 1 || symbol_type == 2) && !is_valid_symbol(symbol_name)){ 
        /* IMPROVE ERRORS*/
        /* checks if symbol is in correct format if data or string*/
        return 0;
    }
    new_node->name = strdup(symbol_name);
    new_node->type = symbol_type;
    new_node->memory_place = memory_place;
    if (!new_node->name || !new_node->type || !new_node->memory_place) {
        /*Handle memory allocation failure*/
        free(new_node->name);
        free(new_node->memory_place);
        free(new_node);
        return 0;
    }
    *(new_node->memory_place) = memory_place;
    new_node->next = NULL;

    if (table->head == NULL) {
        table->head = new_node;
        table->last = new_node;
    } else if (table->last != NULL)
    {
        table->last->next = new_node;
    }
    table->last = new_node;
    return 1; /*Symbol added successfully*/
}

SymbolTable *createSymbolTable()
{
    SymbolTable *table = NULL;
    table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table == NULL)
    {
        /* IMRPOVE ERRORS*/
        /* memory allocation failed*/
    }
    table->head = NULL;
    table->last = NULL;
    return table;
}

void freeSymbolNode(SymbolNode *node) {
    if (node != NULL)
    {
        free(node->symbolName);
        free(node->symbolType);
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