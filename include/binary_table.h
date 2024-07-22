#ifndef __BINARY_TABLE_H__
#define __BINARY_TABLE_H__

typedef struct BinaryNode
{
    int decAddress;
    char *binaryCode;
    char *sourceCode;
    struct BinaryNode *next;
} BinaryNode;

typedef struct BinaryTable
{
    BinaryNode *head;
    BinaryNode *last;
    int length;
} BinaryTable;

BinaryTable* create_binary_table();
void free_binary_table(BinaryTable *table);
void print_binary_table(BinaryTable *table);

#endif /* __BINARY_TABLE_H__ */
