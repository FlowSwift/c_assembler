#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "binary_table.h"

BinaryTable *create_binary_table()
{
    BinaryTable *table = NULL;
    table = (BinaryTable *)malloc(sizeof(BinaryTable));
    if (table == NULL)
    {
        /* IMRPOVE ERRORS*/
        /*Memory allocation failed*/
        return 0;
    }
    table->head = NULL;
    table->last = NULL;
    table->length = 0;
    return table;
}
void free_binary_table(BinaryTable *table)
{
    if (table == NULL)
    {
        return;
    }
    BinaryNode *current = NULL;
    current = table->head;
    BinaryNode *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->binaryCode);
        free(current->sourceCode);
        free(current);
        current = next;
    }
    free(table);
}

void print_binary_table(BinaryTable *table)
{
    if (table == NULL)
    {
        printf("BinaryTable is NULL\n");
    }
    BinaryNode *current = NULL;
    current = table->head;
    while (current != NULL)
    {
        printf("Address: %d, Binary Code: %s, Source Code: %s\n",
        current->decAddress, current->binaryCode, current->sourceCode);
        current = current->next;
    }
}