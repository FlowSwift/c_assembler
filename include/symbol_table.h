#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/**
 * @enum SymbolType
 * @brief An enumeration representing the different types of symbols in assembly language.
 *
 * This enum is used to categorize symbols (labels) in the symbol table. It defines the role or type
 * of the symbol within the assembly program, such as whether the symbol is an instruction, data,
 * or a special directive like `.entry` or `.extern`.
 *
 * @var TYPE_LABEL_DEF
 * Represents a label definition.
 *
 * @var TYPE_INSTRUCTION
 * Represents an instruction symbol.
 *
 * @var TYPE_DATA
 * Represents a data symbol. 
 *
 * @var TYPE_STRING
 * Represents a string symbol.
 *
 * @var TYPE_ENTRY
 * Represents an entry symbol. This type is used for symbols that are marked with the `.entry` directive,
 * indicating that they should be included in the entry table.
 *
 * @var TYPE_EXTERN
 * Represents an external symbol. This type is used for symbols that are marked with the `.extern` directive,
 * indicating that they are defined outside of the current assembly file.
 */
typedef enum
{
    TYPE_LABEL_DEF = 0,
    TYPE_INSTRUCTION,
    TYPE_DATA,
    TYPE_STRING,
    TYPE_ENTRY,
    TYPE_EXTERN
} SymbolType;

#include "macros.h"

/**
 * @struct SymbolNode
 * @brief A structure representing a symbol (label) in the assembly language.
 * Each symbol node stores information about a label - name, type (instruction, data, or string),
 * label type (label definition, entry, or extern), and its memory address.
 *
 * @var SymbolNode::name
 * The name of the symbol (label).
 *
 * @var SymbolNode::type
 * The type of the symbol that is defined - command or directive(.data or .string). Only relevent when label is defined.
 *
 * @var SymbolNode::label_type
 * The type of the label - definition, entry, or extern. 
 *
 * @var SymbolNode::memory_place
 * The memory location associated with the symbol.
 *
 * @var SymbolNode::next
 * A pointer to the next symbol node in the linked list.
 */
typedef struct SymbolNode
{
    char *name;
    int type; /* Instruction, data or string */
    int label_type; /* Label def, entry or extern */
    int memory_place;
    struct SymbolNode *next;
} SymbolNode;

/**
 * @struct SymbolTable
 * @brief A structure representing the symbol table in the assembly language.
 * The symbol table stores all the defined symbols (labels) as a linked list of `SymbolNode` structures.
 *
 * @var SymbolTable::head
 * A pointer to the first symbol node in the list.
 *
 * @var SymbolTable::last
 * A pointer to the last symbol node in the list.
 */
typedef struct SymbolTable
{
    SymbolNode *head;
    SymbolNode *last;
} SymbolTable;

/**
 * @brief Searches for a symbol name in the symbol table.
 * This function searches if the symbol name given is already in symbol table, by name.
 *
 * @param table A pointer to the symbol table.
 * @param symbol_name The name of the symbol to search for.
 *
 * @return A pointer to the found symbol node if it exists, or NULL if the symbol is not found.
 */
SymbolNode *is_symbol_in_table(SymbolTable *table, char *symbol_name);

/**
 * @brief Checks if a symbol name is valid in allowed format.
 * This function checks if the given symbol name is valid by this rules for symbol names:
 * - name length under 31 characters
 * - valid characters, only alphabetic characters allowed
 * - name not relevnt for Assembly language - not an opcode, opernd, macro name etc.
 *
 * @param macro_head A pointer to the macro list for checking macro name conflicts.
 * @param label The symbol name to validate.
 *
 * @return 0 if the symbol name is valid, or an error code if invalid.
 */
int is_valid_symbol(struct macros *head, char *label);

/**
 * @brief Adds a new symbol to the symbol table.
 * It first checks if the symbol is valid and does not already exist, then inserts the symbol with the given properties.
 *
 * @param table A pointer to the symbol table.
 * @param symbol_name The name of the symbol to add.
 * @param type The type of the symbol (instruction, data, or string).
 * @param label_type The label type of the symbol (label definition, entry, or extern).
 * @param memory_place The memory address associated with the symbol.
 * @param macro_head A pointer to the macro list for checking macro name conflicts.
 *
 * @return 0 if the symbol was added successfully, or an error code if an error occurred.
 */
int add_symbol_to_table(SymbolTable *table, char *symbol_name, int type, int label_type, int memory_place, struct macros *macro_head);

/**
 * @brief Creates a new, empty symbol table.
 * This function allocates memory for a new symbol table and initializes it with an empty list of symbols.
 *
 * @return A pointer to the newly created symbol table, or NULL if memory allocation failed.
 */
SymbolTable *createSymbolTable();

/**
 * @brief Frees a symbol node from memory.
 *
 * This function frees the memory allocated for a given symbol node, including its name.
 *
 * @param node A pointer to the symbol node to free.
 */
void freeSymbolNode(SymbolNode *node);

/**
 * @brief Frees the entire symbol table.
 *
 * This function frees all the symbol nodes in the symbol table and the memory allocated for the table itself.
 *
 * @param table A pointer to the symbol table to free.
 */
void freeSymbolTable(SymbolTable *table);

#endif /* __SYMBOL_TABLE_H__ */