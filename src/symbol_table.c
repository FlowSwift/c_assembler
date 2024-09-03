
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "constant.h"
#include "first_pass.h"
#include "error_handler.h"
#include "macros.h"
#include "util.h"

SymbolNode *is_symbol_in_table(SymbolTable *table, char *symbol_name)
{
    SymbolNode *current = NULL;
    current = table->head;
    while (current != NULL)
    {
        if ((strcmp(current->name, symbol_name) == 0))
        {
            return current; /* Symbol found*/
        }
        current = current->next;
    }
    return NULL; /*Symbol not found*/
}

int is_valid_symbol(struct macros *macro_head, char *label)
{
    int i = 1;
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
    if (check_if_opcode(label) != 0)
    {
        error_flag = ERROR_SYMBOL_NAME_IS_OPCODE;
        return error_flag;
    }
    /*check if label is register name*/
    if (!valid_reg_name(label))
    {
        error_flag = ERROR_SYMBOL_NAME_IS_REGISTER;
        return error_flag;
    }
    /*check if label is macro name*/
    if (is_existing_macro(macro_head, label) != NULL)
    { /*found label in macro table*/
        error_flag = ERROR_SYMBOL_NAME_IS_MACRO;
        return error_flag;
    }
    return error_flag; /* 0 if success*/
}

int add_symbol_to_table(SymbolTable *table, char *symbol_name, int type, int label_type, int memory_place, struct macros *macro_head)
{
    SymbolNode *new_node = NULL, *current = table->head;
    ErrorCode error_flag = 0; /*assume success*/
    new_node = is_symbol_in_table(table, symbol_name);
    if (new_node != NULL)
    {
        if ((label_type == TYPE_LABEL_DEF) && (new_node->memory_place != 0))
        {
            error_flag = ERROR_SYMBOL_DEFINED_TWICE;
            return error_flag;
        }
        else if ((label_type == TYPE_ENTRY) && (new_node->label_type == TYPE_LABEL_DEF))
        {
            new_node->label_type = label_type;
            return error_flag;
        }
        else if ((new_node->label_type == TYPE_ENTRY) && (label_type == TYPE_LABEL_DEF))
        {
            new_node->memory_place = memory_place;
            new_node->type = type;
            return error_flag;
        }
        else if ((new_node->label_type == TYPE_EXTERN) && (label_type == TYPE_ENTRY))
        {
            error_flag = ERROR_SYMBOL_ALREADY_EXTERN;
            return error_flag;
        }
        else if ((new_node->label_type == TYPE_ENTRY) && (label_type == TYPE_EXTERN))
        {
            error_flag = ERROR_SYMBOL_ALREADY_EXTERN;
            return error_flag;
        }
        else
        {
            error_flag = ERROR_SYMBOL_DEF_ERROR;
            return error_flag; /*Symbol already exists*/
        }
    }
    else
    {
        /* Create a new node */
        new_node = (SymbolNode *)malloc(sizeof(SymbolNode));
        if (new_node == NULL)
        {
            error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
            return error_flag;
        }
        new_node->name = NULL;
        new_node->next = NULL;
        if (is_valid_symbol(macro_head, symbol_name) != 0)
        {
            error_flag = ERROR_SYMBOL_SYNTAX_IS_WRONG; /* checks if symbol is in correct format */
            return error_flag;
        }
        new_node->name = strdup1(symbol_name);
        new_node->type = type;
        new_node->label_type = label_type;
        new_node->memory_place = memory_place;
        /* Add the new node to the table */
        if (table->head == NULL)
        {
            /* The list is empty, so the new node is the head */
            table->head = new_node;
        }
        else
        {
            /* Traverse the list to find the last node */
            while (current->next != NULL)
            {
                current = current->next;
            }
            /* Link the new node at the end of the list */
            current->next = new_node;
        }
    }
    return error_flag; /*if 0, Symbol added successfully*/
}

SymbolTable *createSymbolTable()
{
    printf("Creating symbol table\n");
    SymbolTable *table = NULL;
    table = (SymbolTable *)malloc(sizeof(SymbolTable));
    ErrorCode error_flag = 0; /*assume success*/
    if (table == NULL)
    {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        handle_error(error_flag, 0);
        return NULL;
    }
    table->head = NULL;
    table->last = NULL;
    return table;
}

void freeSymbolNode(SymbolNode *node)
{
    if (node != NULL)
    {
        free(node->name);
        free(node);
    }
}

void freeSymbolTable(SymbolTable *table)
{
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
        free(table);
    }
}