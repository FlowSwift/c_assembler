#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "command_handler.h"
#include "error_handler.h"
#include "constant.h"
#include "util.h"

Opcode OPCODES[] = {
    {0, "mov", 2},
    {1, "cmp", 2},
    {2, "add", 2},
    {3, "sub", 2},
    {4, "lea", 2},
    {5, "clr", 1},
    {6, "not", 1},
    {7, "inc", 1},
    {8, "dec", 1},
    {9, "jmp", 1},
    {10, "bne", 1},
    {11, "red", 1},
    {12, "prn", 1},
    {13, "jsr", 1},
    {14, "rts", 0},
    {15, "stop", 0}};

int check_type(Operand *operand, struct macros *macro_head)
{
    ErrorCode error_flag = 0; /*Assume success*/
    int num = 0;              /*number if addressing type is immediate (0)*/
    int len = strlen(operand->value) - 1;
    if ((operand->value[0] == '\0'))
    {
        error_flag = ERROR_NOT_ENOUGH_OPERANDS;
        return error_flag;
    }
    switch (operand->value[0])
    {
    case '#': /*Immediate addressing type(0).*/
        /*Remove '#' and check if the remaining part is a valid integer*/
        memmove(operand->value, operand->value + 1, len);
        operand->value[len] = '\0';
        error_flag = is_valid_integer(operand->value);
        if (error_flag == 0)
        {
            {
                num = atoi(operand->value); /*convert string to int*/
                if (num > MAX_OPERAND_INT)
                {
                    error_flag = ERROR_INTEGER_VALUE_TOO_BIG; /*integer value too big*/
                }
                else if (num < MIN_OPERAND_INT)
                {
                    error_flag = ERROR_INTEGER_VALUE_TOO_SMALL; /*integer value too small*/
                }
            }
            operand->type = 0;
        }
        else /*value wasn't integer.*/
        {
            return error_flag;
        }
        break;
    case '*': /*indirect register addressing type(2).*/
        /*Remove '*' and check if the remaining part is a valid register*/
        memmove(operand->value, operand->value + 1, len);
        operand->value[len] = '\0';
        error_flag = valid_reg_name(operand->value);
        if (error_flag == 0)
        {
            operand->type = 2;
            break;
        }
        else
        {
            return error_flag;
        }
    case 'r': /*direct register addressing type(3).*/
        error_flag = valid_reg_name(operand->value);
        if (error_flag == 0)
        {
            operand->type = 3;
            break;
        }
        else
        {
            return error_flag;
        }
    default: /*if isn't one of 0,2,3 addressing type than assume it is direct and the operand is symbol.*/
        error_flag = is_valid_symbol(macro_head, operand->value);
        if (error_flag == 0)
        {
            operand->type = 1; /*Default is label but it's existence will be checked in second pass */
            break;
        }
        else /*not valid symbol name*/
        {
            return error_flag;
        }
    }
    return error_flag; /*0 ->success*/
}

int get_opcode_operands(char *instruction)
{
    int i = 0;
    for (i = 0; i < OP_CODE_ARRAY_SIZE; i++)
    {
        if (strcmp(instruction, OPCODES[i].name) == 0)
        {
            return OPCODES[i].numOfOperands; /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int get_opcode_code(char *instruction)
{
    int i = 0;
    for (i = 0; i < OP_CODE_ARRAY_SIZE; i++)
    {
        if (strcmp(instruction, OPCODES[i].name) == 0)
        {
            return OPCODES[i].code; /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int operand_parser(AssemblyLine *parsedLine, struct macros *macro_head)
{
    char *ptr_in_line = NULL;
    char *start = NULL;
    char *operandValue = NULL;
    ErrorCode error_flag = 0; /*Assume success*/
    /*helper variables:*/
    int opcode_code = -1, operandCount = 0, num_operands_allowed = 0, operandLen = 0;
    ptr_in_line = parsedLine->operands;
    opcode_code = get_opcode_code(parsedLine->instruction); /*gets the value of the opcode*/
    parsedLine->opcode_code = opcode_code;
    num_operands_allowed = get_opcode_operands(parsedLine->instruction); /*gets how many operands allowed for Opcode*/

    if (num_operands_allowed == -1)
    { /*insturction not found*/
        error_flag = ERROR_INSTRUCTION_NOT_VALID;
        return error_flag;
    }
    printf("num_operands_allowed: %d\n", num_operands_allowed); /*delete*/
    if (num_operands_allowed == 0)
    { /*no operands allowed*/
        parsedLine->srcOperand = NULL;
        parsedLine->destOperand = NULL;
        return error_flag;
    }
    /*if one or more operands allowed*/
    parsedLine->destOperand = (Operand *)malloc(sizeof(Operand));
    parsedLine->srcOperand = (Operand *)malloc(sizeof(Operand));
    if ((parsedLine->srcOperand == NULL) || (parsedLine->destOperand == NULL))
    {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
    }
    parsedLine->srcOperand->value = NULL;
    parsedLine->destOperand->value = NULL;
    /*goes through Operands in parsed line and sets Operands to their place in the Assembly line structure.*/
    while ((*ptr_in_line != '\0') && (operandCount < num_operands_allowed))
    {
        while ((*ptr_in_line != '\0') && (isspace(*ptr_in_line)))
            ptr_in_line++;                            /* Skip whitespace */
        if (*ptr_in_line == ',' && operandCount == 0) /*if another operand is found but should be 0.*/
        {
            error_flag = ERROR_WRONG_OPERAND_SYNTAX;
            return error_flag;
        }
        start = ptr_in_line;
        operandLen = 0;
        while ((*ptr_in_line != '\0') && (*ptr_in_line != ',') && (!isspace(*ptr_in_line)))
        {
            ptr_in_line++;
            operandLen++;
        }
        while ((*ptr_in_line != '\0') && (isspace(*ptr_in_line)))
            ptr_in_line++; /* Skip whitespace */
        operandValue = (char *)malloc(operandLen + 1);
        if (operandValue == NULL)
        {
            error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
            return error_flag;
        }
        strncpy(operandValue, start, operandLen);
        operandValue[operandLen] = '\0';
        operandValue = trim_whitespace(operandValue); /*already adds operandValue[operandLen] = '\0';*/
        if (num_operands_allowed == 2 && operandCount == 0)
        { /*if while reading first operand where should be two -> assign to srcOperand*/
            parsedLine->srcOperand->value = operandValue;
            error_flag = check_type(parsedLine->srcOperand, macro_head);
            if (error_flag != 0)
            { /*inserts type of miun to parsedLine and checks errors*/
                return error_flag;
            }
        }
        else if (num_operands_allowed == 1 || (num_operands_allowed == 2 && operandCount == 1))
        { /*if only one operand allowed or if two and this is second operand -> assign to destOperand .*/
            parsedLine->destOperand->value = operandValue;
            error_flag = check_type(parsedLine->destOperand, macro_head);
            if (error_flag != 0)
            { /*inserts type of miun to parsedLine and checks errors*/
                return error_flag;
            }
        }
        else
            continue;
        operandCount++;
        if ((*ptr_in_line == ','))
        {
            if (operandCount == num_operands_allowed)
            {
                error_flag = ERROR_TOO_MANY_OPERANDS;
                return error_flag;
            }
            ptr_in_line++; /* Skip comma */
        }
    }
    /* Check for extra characters after the last operand (up to 2), if num_operands == 0 will also arrive here and check*/
    while (*ptr_in_line != '\0')
    {
        if (!isspace(*ptr_in_line))
        {
            /*there are chars after last operand or operands when needed to be 0*/
            error_flag = ERROR_TOO_MANY_OPERANDS;
            return error_flag;
        }
        ptr_in_line++;
    }
    if ((num_operands_allowed != operandCount) || (operandCount == 0 && parsedLine->operands != NULL))
    {
        error_flag = ERROR_INSTRUCTION_NOT_VALID;
        return error_flag;
    }
    /* Assign operands to parsedLine */
    if (num_operands_allowed == 1)
    {
        parsedLine->srcOperand->type = -1;
        parsedLine->srcOperand->value = '\0';
        parsedLine->destOperand = parsedLine->destOperand;
    }
    if (num_operands_allowed == 0 && parsedLine->operands == NULL) /*if noe operands found, set type to -1 and value to 0.*/
    {
        parsedLine->srcOperand->type = -1;
        parsedLine->srcOperand->value = '\0';
        parsedLine->destOperand->type = -1;
        parsedLine->destOperand->value = '\0';
    }
    else
    { /*set Operands in assembly line.*/
        parsedLine->srcOperand = parsedLine->srcOperand;
        parsedLine->destOperand = parsedLine->destOperand;
    }

    error_flag = check_valid_operands(parsedLine); /*checks if operand types are allowed for the command.*/
    printf("ERROR FLAG %d\n", error_flag);         /*delete*/
    return error_flag; /* 0 -> SUCCESS. the instructions matched the operands succefully and all was allocated in parsedLine */
}

int check_valid_operands(AssemblyLine *parsedLine)
{
    /*check if miun type of src and dest is valid to instruction*/
    int type_miun_src = parsedLine->srcOperand->type;
    int type_miun_dest = parsedLine->destOperand->type;
    int opcode_code = parsedLine->opcode_code;
    ErrorCode error_flag = 0; /*Assume success*/
    switch (opcode_code)
    {
    /* -1 if instruction is not found in 16 allowed OPCODES. */
    case -1:
        error_flag = ERROR_INSTRUCTION_NOT_VALID; /*cannot find insturction*/
        break;
    /*mov (0), add(2), sub(3) - {0,1,2,3) types allowed for src and {1,2,3} for dest*/
    case 0:
    case 2:
    case 3:
        printf("type_miun_src: %d\n", type_miun_src);   /*delete*/
        printf("type_miun_dest: %d\n", type_miun_dest); /*delete*/
        if (!((type_miun_src >= 0 && type_miun_src <= 3) && (type_miun_dest >= 1 && type_miun_dest <= 3)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
        break;
    /*cmp(1) - {0,1,2,3} allowed for src and dest*/
    case 1:
        if (!((type_miun_src >= 0 && type_miun_src <= 3) && (type_miun_dest >= 0 && type_miun_dest <= 3)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
        break;
    /*lea(4) - {1} allowed for src and {1,2,3} for dest*/
    case 4:
        if (!((type_miun_src == 1) && (type_miun_dest >= 1 && type_miun_dest <= 3)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
        break;
    /*clr(5), not(6), inc(7), dec(8), red(11) - only dest allowed - {1,2,3}*/
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
        if (!((type_miun_src == -1)) && (type_miun_dest >= 1 && type_miun_dest <= 3))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
        break;
    /*jmp (9), bne(10), jsr(13) - only dest allowed - {1,2}*/
    case 9:
    case 10:
    case 13:
        if (!((type_miun_src == -1) && (type_miun_dest >= 1 && type_miun_dest <= 2)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
        break;
    /*prn(12) - only dest allowed - {0,1,2,3}*/
    case 12:
        if (!((type_miun_src == -1) && (type_miun_dest >= 0 && type_miun_dest <= 3)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
    /*rts(14), stop(15) - not expecting any operands. */
    case 14:
    case 15:
        if (!((type_miun_src == -1)) && (!(type_miun_dest == -1)))
        {
            error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
        }
    default:
        break;
    }
    return error_flag; /* 0 -> SUCCESS*/
}

int handle_instruction(AssemblyLine *parsedLine, SymbolTable *symbol_table, BinaryLine **instruction_binary_table, int *IC, struct macros *macro_head, int line)
{
    ErrorCode error_flag = 0;                            /*Assume success*/
    error_flag = operand_parser(parsedLine, macro_head); /*checks if OPCODE is legal and Operands are as should be, by types of miun*/
    if (error_flag != 0)
    {
        freeOperand(parsedLine->srcOperand);
        parsedLine->srcOperand = NULL;
        freeOperand(parsedLine->destOperand);
        parsedLine->destOperand = NULL;
        return error_flag;
    }
    printf("CHECK %s\n", parsedLine->instruction); /*delete*/
    /*converts to binary a valid parsed line.*/
    convert_instruction_to_binary_code(parsedLine, instruction_binary_table, line, IC);
    return error_flag; /* 0 -> SUCCESS*/
}