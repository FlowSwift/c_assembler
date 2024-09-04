#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__
/*#define MAX_LABEL_LEN 31*/
#include "line_parser.h"
#include "symbol_table.h"
#include "binary_conversion.h"

/**
 * @struct Opcode
 * @brief A structure representing an opcode in assembly language.
 *
 * This structure holds information about an opcode, including its name, the opcode code, and the number of operands it accepts.
 *
 * @var Opcode::code
 * The numeric code associated with the opcode (given by the array of OPCODES).
 *
 * @var Opcode::name
 * The name of the opcode ("mov", "add" ect.).
 *
 * @var Opcode::numOfOperands
 * The number of operands the opcode requires.
 */
typedef struct
{
    int code;
    char *name;
    int numOfOperands;
} Opcode;

/**
 * @enum Register
 * @brief Enumeration of registers in the assembly language, in this program optional registers are r0 to r7.
 */
typedef enum
{
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
} Register;

/*using OPCODES table to format all aloowed opcodes and there name, number and num of operands allowed.*/
extern Opcode OPCODES[];

/**
 * @brief Performs the first pass on the assembly file.
 * This function foes through each line of the assembly code, parses it and converts it into binary.
 * The function updates the symbol table with the symbols defined in the file.
 *
 * @param file_name The name of the assembly file to process.
 * @param macro_head A pointer to the head of the macro list.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table for instructions and directives.
 * @param IC Instruction counter, updated during processing. For direct instructions. 
 * @param DC Data counter, updated during processing. For .string and .data directives.
 *
 * @return 0 on success, or an error code on failure.
 */
int first_pass(char *file_name, struct macros *macro_head, SymbolTable *symbolTable, BinaryLine **binary_table, int *IC, int *DC);



/**
 * @brief Checks the addressing type(miun) of operand and updates it in its structure.
 * The function determines the type of addressing based on its format and based on the type updates the value. 
 *
 * @param operand A pointer to the operand to update.
 * @param macro_head A pointer to the head of the macro list for validation(needed to check if label name is valid).
 *
 * @return 0 on success, or an error code on failure.
 */
int check_type(Operand *operand, struct macros *macro_head);

/**
 * @brief Gets the number of operands allowed for the given instruction.
 * Searches the opcode table to find how many operands the instruction accepts.
 *
 * @param instruction A string containing the instruction.
 * @return The number of operands for the instruction, or -1 if invalid.
 */
int get_opcode_operands(char *instruction);

/**
 * @brief Gets the opcode code for the given instruction.
 * Searches the opcode table to retrieve the numeric code of the instruction.
 *
 * @param instruction A string containing the instruction.
 *
 * @return The opcode code of the instruction, or -1 if invalid.
 */
int get_opcode_code(char *instruction);

/**
 * @brief Parses operands for the given assembly line of instruction type.
 * Validates the format of the line - the instruction is one of the defined opcodes, checks the type of operands,
 * and ensures that the number of operands is correct for the instruction.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param macro_head A pointer to the head of the macro list for validation(needed to check if label name is valid).
 *
 * @return 0 on success, or an error code on failure.
 */
int operand_parser(AssemblyLine *parsedLine, struct macros *macro_head);

/**
 * @brief Checks if the source and destination operands are valid for the instruction.
 * Ensures that the operand addressing types (miun) are valid for the given instruction.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 *
 * @return 0 on success, or an error code on failure.
 */
int check_valid_operands(AssemblyLine *parsedLine);


/**
 * @brief Handles the processing of an instruction, parsing into Operands, validates the format of the instruction
 * inserts it into the binary table, and adds defined label to the symbol table.
 * instruction structure is: <label:-optional> <OPCODE> <operand :optional>, <operand :optional>
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbol_table A pointer to the symbol table.
 * @param instruction_binary_table A pointer to the instruction binary table.
 * @param IC A pointer to the instruction counter.
 * @param macro_head A pointer to the head of the macro list.
 * @param line The line number being processed.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_instruction(AssemblyLine *parsedLine, SymbolTable *symbol_table, BinaryLine **instruction_binary_table, int *IC, struct macros *macro_head, int line);

/**
 * @brief Handles a `.string` directive line.
 * String directive lines structure is: <label:-optional> .string "<data>". 
 * The data can be any character as long it is between teo "".
 * Function processes a string directive, converts it into binary code, and updates the data counter.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param directive_binary_table A pointer to the directive binary table.
 * @param line_number The line number being processed.
 * @param DC A pointer to the data counter.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_string_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line_number, int *DC);

/**
 * @brief Handles a `.data` directive line.
 * Data directive lines structure is: <label:-optional> .data <numeric values devided by `,`>.
 * The values must be whole numbers.
 * Function processes a data directive, converting the data into binary, and updates the data counter.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param directive_binary_table A pointer to the directive binary table.
 * @param line_number The line number being processed.
 * @param DC A pointer to the data counter.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_data_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **directive_binary_table, int line, int *DC);

/**
 * @brief Handles a `.extern` directive line.
 * Processes an extern directive, adds the symbol to the symbol table with default memory(0) and label type EXTERN.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table.
 * @param macro_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_extern_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);

/**
 * @brief Handles a `.entry` directive line.
 * Processes an entry directive, adds the symbol to the symbol table with default memory(0) and label type ENTRY.
 *
 * @param parsedLine A pointer to the parsed assembly line structure.
 * @param symbolTable A pointer to the symbol table.
 * @param binary_table A pointer to the binary table.
 * @param macro_head A pointer to the head of the macro list.
 *
 * @return 0 on success, or an error code on failure.
 */
int handle_entry_directive(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head);
#endif /* __FIRST_PASS_H__ */