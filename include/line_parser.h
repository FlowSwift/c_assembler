#ifndef __LINE_PARSER_H__
#define __LINE_PARSER_H__
/**
 * @struct Operand
 * @brief A structure representing an operand in parsed assembly line.
 * 
 * @var Operand::value
 * The value of the operand. If the operand is a register, the value will be 
 * converted to a number. If there is no operand, value is 0.`
 *
 * @var Operand::type
 * The addressing type (miun) of the operand, with values range from {0, 1, 2, 3} 
 * for different operand types. A value of -1 indicates no operand is present.
 */
typedef struct Operand
{
    char *value; 
    int type;    
} Operand;


/**
 * @struct AssemblyLine
 * @brief A structure representing a parsed line of assembly code.
 *
 * @var AssemblyLine::label
 * The name of the label is defined in the assembly line. NULL if no label is defined.
 * Should be in format <label_name:>. 
 *
 * @var AssemblyLine::instruction
 * The instruction part of the assembly line, which can be a direct command (mov,add, inc, dec, stop) or a directive (.data, .string, .entry or .extern).
 *
 * @var AssemblyLine::operands
 * A string representing the operands in the assembly line.
 *
 * @var AssemblyLine::opcode_code
 * The opcode corresponding to the instruction, representing the encoding of the command, important for binary conversion.
 * Relevant only for command line.
 *
 * @var AssemblyLine::srcOperand
 * A pointer to the source operand in type structure `Operand`.Contains the value and type of the source operand, if any.
 * Relevant only for command line.
 *
 * @var AssemblyLine::destOperand
 * A pointer to the destination operand in type structure `Operand`. Ccontains the value and type of the destination operand, if any.
 * If there is only one Operand it will be in the destination Operand.
 * Relevant only for command line.
 */
typedef struct AssemblyLine
{
    char *label;
    char *instruction; /* instruction can be a direct command(inc, dec, stop etc) or directive */
    char *operands;
    int opcode_code;
    Operand *srcOperand;
    Operand *destOperand;
} AssemblyLine;

/**
 * @brief Function parses a line of assembly code in AssemblyLine structure with 3 components.
 *
 * This function takes a single line of assembly code and extracts the label (if added or NULL), instruction, and operands, if present. 
 * It returns these components as an AssemblyLine structure.
 * The label should end in ":" and cannot be defined more than once. 
 * The instruction and operands are separated by a whitespace. The operands are divided by ",".
 * The instruction in this stage can be direct instruction type(command) or directive line and it doesnt check if the line is in va;id formmating. 
 *
 * @param line A assembly line read in the first pass function. 
 * @param line_number The line number handeled. 
 * @return An AssemblyLine structure with the parsed components:
 *         - label: The label part of the line, or NULL if no label is found.
 *         - instruction: The instruction part of the line.
 *         - operands: The operands part of the line, or NULL if no operands are present.
 *         - srcOperand: Currently set to NULL (not parsed in this function).
 *         - destOperand: Currently set to NULL (not parsed in this function).
 */
AssemblyLine parseAssemblyLine(char *line, int line_number);

/**
 * @brief Function prints a parsed Asssembly line of assembly code by its components.
*/ 
void printAssemblyLine(const AssemblyLine *parsedLine);

/**
 * @brief Function frees memory allocated for parsed Assembly line after usage for binary conversion or if the program fails. 
*/ 
void freeAssemblyLine(AssemblyLine *line);

/**
 * @brief Function frees memory allocated for Operands parsed Assembly line after usage for binary conversion or if the program fails. 
*/ 
void freeOperand(Operand *operand);

#endif /* __LINE_PARSER_H__ */