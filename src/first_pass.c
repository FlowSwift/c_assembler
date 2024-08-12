#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
#include "symbol_table.h"
#include "binary_table.h"
#include "util.h"
#include "constant.h"
#include "error_handler.h"
#include "macros.h"
#include "binary_conversion.h"

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

int firstPass(char *file_name, struct macros *macro_head, SymbolTable *symbolTable, BinaryLine **binaryTable, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    FILE *amfile = fopen(file_name, "r");
    AssemblyLine parsedLine;
    SymbolNode *current = NULL;
    int lineNumber = 0;
    int is_symbol = 0;
    int temp_memory_place = 0;
    ErrorCode error_flag = 0; /*assume success*/
    while (fgets(line, MAX_LINE_LENGTH, amfile) != NULL)
    {
        printf("----------------\n");
        error_flag = 0;
        temp_memory_place = 0;
        lineNumber++;
        if ((is_empty_line(line) == 0) || (is_commented_line(line) == 0))
        { /*continue to next line if line is empty or commented.*/
            continue;
        }
        parsedLine = parseAssemblyLine(line); /* Parse line for this format: label{optional},instruction, operands.*/
        printf("Parsed Line %d:\n", lineNumber);
        printAssemblyLine(&parsedLine);
        if (parsedLine.label != NULL)
        {                  /*a label is defined. NULL when not.*/
            is_symbol = 1; /*update is_sunbol flag*/
        }
        if (parsedLine.instruction[0] == '.')
        { /* if '.' is found it is directive line.*/
            if (strcmp(parsedLine.instruction, DATA_DIRECTIVE) == 0)
            { /* if data*/
                temp_memory_place = *DC;
                error_flag = handleDataDirective(&parsedLine, symbolTable, binaryTable, DC);
                if (is_symbol && (error_flag == 0))
                {                                                                                                              /*data was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_DATA, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 1: data*/
                }
            }
            else if (strcmp(parsedLine.instruction, STRING_DIRECTIVE) == 0)
            { /* if string*/
                temp_memory_place = *DC;
                error_flag = handleStringDirective(&parsedLine, symbolTable, binaryTable, DC);
                if (is_symbol && (error_flag == 0))
                {                                                                                                                /*string was in correct format and has symbol definition*/
                    error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_STRING, temp_memory_place, macro_head); /*Checks correct syntax in function. symbol type is 2: string*/
                }
            }
            else if (strcmp(parsedLine.instruction, EXTERN_DIRECTIVE) == 0)
            { /* if extern*/
                error_flag = handleExternDirective(&parsedLine, symbolTable, binaryTable, macro_head);
            }
            else if (strcmp(parsedLine.instruction, ENTRY_DIRECTIVE) == 0)
            { /* if entry*/
                error_flag = handleEntryDirective(&parsedLine, symbolTable, binaryTable, macro_head);
            }
            else
            { /*if not one of defined directives.*/
                error_flag = ERROR_NOT_DEFINED_DIRECTIVE;
            }
        }
        else
        { /* assume line is command - maybe also label is defined*/
            temp_memory_place = *IC;
            error_flag = handle_instruction(&parsedLine, symbolTable, binaryTable, IC, macro_head, lineNumber); /*label is defined inside, also makes binary*/
            printf("THIS THIS THIS %d\n", error_flag);
            if (is_symbol && (error_flag == 0))
            {                                                                                                                   /* has label and is in regular instruction format.*/
                error_flag = add_symbol_to_table(symbolTable, parsedLine.label, TYPE_LABEL_DEF, temp_memory_place, macro_head); /*also checks if name is legal, symbol gets IC place*/
            }
        }
        if (error_flag != 0)
        { /*if something failed*/
            handle_error(error_flag, lineNumber);
        }
        is_symbol = 0; /*return symbol flag to 0*/
        /* check if memory overflow*/
        if ((*IC + *DC) > MAX_MEMORY_WORDS)
        {
            error_flag = ERROR_MEMORY_OVERFLOW;
            handle_error(error_flag, lineNumber);
            return error_flag;
        }
        if (get_opcode_code(parsedLine.instruction) != -1)
        {
            if (parsedLine.srcOperand != NULL)
            {
                printf("Source Operand: %s\n", parsedLine.srcOperand->value);
                printf("Type of src %d\n", parsedLine.srcOperand->type);
            }
            if (parsedLine.destOperand != NULL)
            {
                printf("Destination Operand: %s\n", parsedLine.destOperand->value);
                printf("Type of dest %d\n", parsedLine.destOperand->type);
            }
        }
        freeAssemblyLine(&parsedLine);
    }
    int i = 1;
    char bin[16];
    while (*binaryTable != NULL)
    {
        printf("Binary Line: %d\n", i);
        decimal_to_binary((*binaryTable)->binary_code, bin, 16);
        printf("Binary Line: %s\n", bin);
        printf("Labal: %s\n", (*binaryTable)->label);
        printf("Binary code: %d\n", (*binaryTable)->binary_code);
        printf("Original line number: %d\n", (*binaryTable)->original_line_number);
        i++;
        printf("---------------------\n");
        *binaryTable = (*binaryTable)->next;
    }
    current = symbolTable->head;
    while (current != NULL)
    {
        if (current->type == 1) /* data type is {data}*/
        {
            current->memory_place += *IC;
        }
        current = current->next;
    }
    SymbolNode *current_symbol = symbolTable->head;
    while (current_symbol != NULL) {
        printf("----------------\n");
        printf("Symbol: %s\n", current_symbol->name);
        printf("Type: %d\n", current_symbol->type);
        printf("Memory place: %d\n", current_symbol->memory_place);
        current_symbol = current_symbol->next;
    }
    fclose(amfile);
    return error_flag; /* 0 -> SUCCESS*/
}

int check_type(Operand *operand, struct macros *macro_head)
{
    ErrorCode error_flag = 0; /*assume success*/
    int len = strlen(operand->value) - 1;
    if ((operand->value[0] == '\0'))
    {
        error_flag = ERROR_NOT_ENOUGH_OPERANDS;
        return error_flag;
    }
    switch (operand->value[0])
    {
    case '#':
        /*Remove '#' and check if the remaining part is a valid integer*/
        memmove(operand->value, operand->value + 1, len);
        operand->value[len] = '\0';
        error_flag = is_valid_integer(operand->value);
        if (error_flag == 0)
        {
            operand->type = 0;
        }
        else
        {
            return error_flag;
        }
        break;
    case '*':
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
    case 'r':
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
    default:
        error_flag = is_valid_symbol(macro_head, operand->value);
        if (error_flag == 0)
        {
            operand->type = 1; /*Default is it's label, will be checked in 2 pass */
            break;
        }
        else
        {
            return error_flag;
        }
    }
    return error_flag; /*0 ->success*/
}

int get_opcode_operands(char *instruction)
{
    /* gets allowed operands of insturction*/
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
    /* gets opcode number of insturction*/
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
    ptr_in_line = parsedLine->operands;
    ErrorCode error_flag = 0; /*assume success*/
    /*helper variables:*/
    int opcode_code = -1, operandCount = 0, num_operands_allowed = 0, operandLen = 0;
    opcode_code = get_opcode_code(parsedLine->instruction); /*get the value of the opcode*/
    parsedLine->opcode_code = opcode_code;
    char *operandValue = NULL;
    Operand *temp_srcOperand = NULL;
    Operand *temp_destOperand = NULL;
    num_operands_allowed = get_opcode_operands(parsedLine->instruction); /*got how many operands allowed for Opcode*/
    if (num_operands_allowed == -1)
    { /*insturction not found*/
        error_flag = ERROR_INSTRUCTION_NOT_VALID;
        return error_flag;
    }
    printf("num_operands_allowed: %d\n", num_operands_allowed);
    if (num_operands_allowed == 0)
    { /*no operands allowed*/
        parsedLine->srcOperand = NULL;
        parsedLine->destOperand = NULL;
        return error_flag;
    }
    temp_srcOperand = (Operand *)malloc(sizeof(Operand));
    temp_destOperand = (Operand *)malloc(sizeof(Operand));
    if ((temp_srcOperand == NULL) || (temp_destOperand == NULL))
    {
        error_flag = ERROR_MEMORY_ALLOCATION_FAILED;
        return error_flag;
    }
    temp_srcOperand->value = NULL;
    temp_destOperand->value = NULL;
    while ((*ptr_in_line != '\0') && (operandCount < num_operands_allowed))
    {
        while ((*ptr_in_line != '\0') && (isspace(*ptr_in_line)))
            ptr_in_line++; /* Skip whitespace */
        if (*ptr_in_line == ',' && operandCount == 0)
        {
            error_flag = ERROR_WRONG_OPERAND_SYNTAX;
            freeOperand(temp_srcOperand);
            freeOperand(temp_destOperand);
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
        { /*while reading first operand where should be two -> assign to srcOperand*/
            temp_srcOperand->value = operandValue;
            error_flag = check_type(temp_srcOperand, macro_head);
            if (error_flag != 0)
            { /*inserts type of miun to parsedLine and checks errors*/
                freeOperand(temp_srcOperand);
                freeOperand(temp_destOperand);
                return error_flag;
            }
        }
        else if (num_operands_allowed == 1 || (num_operands_allowed == 2 && operandCount == 1))
        { /*if only one operand allowed or if two and this is second opperand -> assign to destOperand .*/
            temp_destOperand->value = operandValue;
            error_flag = check_type(temp_destOperand, macro_head);
            if (error_flag != 0)
            { /*inserts type of miun to parsedLine and checks errors*/
                freeOperand(temp_srcOperand);
                freeOperand(temp_destOperand);
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
                freeOperand(temp_srcOperand);
                freeOperand(temp_destOperand);
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
            freeOperand(temp_srcOperand);
            freeOperand(temp_destOperand);
            return error_flag;
        }
        ptr_in_line++;
    }
    if ((num_operands_allowed != operandCount) || (operandCount == 0 && parsedLine->operands != NULL))
    {
        error_flag = ERROR_INSTRUCTION_NOT_VALID;
        freeOperand(temp_srcOperand);
        freeOperand(temp_destOperand);
        return error_flag;
    }
    /* Assign operands to parsedLine */
    if (num_operands_allowed == 1)
    {
        temp_srcOperand->type = -1;
        temp_srcOperand->value = '\0';
        parsedLine->destOperand = temp_destOperand;
    }
    if (num_operands_allowed == 0 && parsedLine->operands == NULL)
    {
        temp_srcOperand->type = -1;
        temp_srcOperand->value = '\0';
        temp_destOperand->type = -1;
        temp_destOperand->value = '\0';
    }
    else
    {
        parsedLine->srcOperand = temp_srcOperand;
        parsedLine->destOperand = temp_destOperand;
    }
    error_flag = check_valid_operands(parsedLine);
    printf("ERROR FLAG %d\n", error_flag);
    if (error_flag != 0)
    {
        freeOperand(temp_srcOperand);
        freeOperand(temp_destOperand);
    }
    return error_flag; /* 0 -> SUCCESS. the instructions matched the operands succefully and all was allocated in parsedLine */
}

int check_valid_operands(AssemblyLine *parsedLine)
{
    /*check if miun type of src and dest is valid to instruction*/
    int type_miun_src = parsedLine->srcOperand->type;
    int type_miun_dest = parsedLine->destOperand->type;
    int opcode_code = parsedLine->opcode_code;
    ErrorCode error_flag = 0; /*assume success*/
    switch (opcode_code)
    {
        /* -1 if instruction is not one of 16 allowed. */
        case -1:
            error_flag = ERROR_INSTRUCTION_NOT_VALID; /*cannot find insturction*/
            break;
        /*mov (0), add(2), sub(3) - {0,1,2,3) types allowed for src and {1,2,3} for dest*/
        case 0:
        case 2:
        case 3:
            if (!(type_miun_src >= 0 && type_miun_src <= 3) && (type_miun_dest >= 1 && type_miun_dest <= 3))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
            break;
        /*cmp(1) - {0,1,2,3} allowed for src and dest*/
        case 1:
            if (!(type_miun_src >= 0 && type_miun_src <= 3) && (type_miun_dest >= 0 && type_miun_dest <= 3))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
            break;
        /*lea(4) - {1,2,3} allowed for src and {1,2,3} for dest*/
        case 4:
            if (!(type_miun_src == 1) && (type_miun_dest >= 1 && type_miun_dest <= 3))
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
            if ((!(type_miun_src == -1)) && (type_miun_dest >= 1 && type_miun_dest <= 3))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
            break;
        /*jmp (9), bne(10), jsr(1) - only dest allowed - {1,2}*/
        case 9:
        case 10:
        case 13:
            if (!(type_miun_src == -1) && (type_miun_dest >= 1 && type_miun_dest <= 2))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
            break;
        /*prn(12) - only dest allowed - {0,1,2,3}*/
        case 12:
            if (!(type_miun_src == -1) && (type_miun_dest >= 0 && type_miun_dest <= 3))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
        /*rts(14), stop(15) - not expecting any operands. */
        case 14:
        case 15:
            if ((!(type_miun_src == -1)) && (!(type_miun_dest == -1)))
            {
                error_flag = ERROR_MIUN_TYPES_DONT_MATCH;
            }
        default:
            break;
    }
    return error_flag; /* 0 -> SUCCESS*/
}

int handle_instruction(AssemblyLine *parsedLine, SymbolTable *symbol_table, BinaryLine **binary_table, int *IC, struct macros *macro_head, int line)
{
    ErrorCode error_flag = 0;                            /*assume success*/
    error_flag = operand_parser(parsedLine, macro_head); /*checks if OPCODE is legal and Operands are as should be, by types of miun*/
    if (error_flag != 0)
    {
        return error_flag;
    }
    printf("CHECK %s\n", parsedLine->instruction);
    convert_instruction_to_binary_code(parsedLine, binary_table, line, IC);
    return error_flag; /* 0 -> SUCCESS*/
}

int calculate_L(int srcType, int dstType)
{
    int L = 1; /* at least 1, the base word */
    if (srcType == -1 && dstType == -1)
    {
        return L;
    }
    if (srcType == 3 && dstType == 3)
    {
        return L + 1;
    }
    if (srcType == 0 || srcType == 3 || srcType == 1)
    {
        L = L + 1;
    }
    if (dstType == 0 || dstType == 3 || dstType == 1)
    {
        L = L + 1;
    }
    if (dstType == 2)
    {
        L = L + 2;
    }
    if (srcType == 2)
    {
        L = L + 2;
    }
    return L;
}

int handleDataDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, int *DC)
{
    char *token = NULL;
    if(parsedLine->operands[strlen(parsedLine->operands)-1] == ',')
    {
        return ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
    }
    token = strtok(parsedLine->operands, ","); /*split operands by ','*/
    int value = 0;
    ErrorCode error_flag = 0; /*assume success*/
    while (token != NULL)
    {
        if (strlen(token) == 0)
        {
            error_flag = ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
            return error_flag;
        }
        token = trim_whitespace(token); /*Trim leading and ending whitespace - use this string for next checks*/
        if (is_valid_integer(token) != 0)
        {
            error_flag = ERROR_WRONG_DATA_DIRECTIVE_SYNTAX;
            return error_flag;
        }
        else
        {                        /*symbol syntax is checked and is integer*/
            value = atoi(token); /*numeral value of the operand*/
        }
        /* Insert the binary */
        /*make binary -
        error = insertToBinaryCodesTable(BinaryTable, *DC, parsedLine, convertIntToBinary(value, BINARY_CODE_LEN), parsedLine->operands);
        */
        if (error_flag != 0)
        {
            return error_flag; /* some error in binary conversion*/
        }
        *DC = *DC + 1;
        token = strtok(NULL, ",");
    }

    return error_flag; /* 0 -> SUCCESS*/
}

int handleStringDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, int *DC)
{
    int stringLen = 0, i = 0;
    char binaryCode[BINARY_CODE_LEN];
    stringLen = strlen(parsedLine->operands);
    memset(binaryCode, '\0', sizeof(binaryCode));
    ErrorCode error_flag = 0; /*assume success*/
    if (is_valid_string(parsedLine->operands) != 0)
    { /*doesnt start and end with quotation.*/
        error_flag = ERROR_STRING_SYNTAX_NOT_VALID;
        return error_flag;
    }
    /*go through string without ""*/
    for (i = 1; i < stringLen - 1; i++)
    {
        /* converts the ASCII value of the character to a binary string */
        /*make binary:
        intToBinaryRes = convertIntToBinary((int)parsedLine->operands[i], BINARY_CODE_LEN);
        strcpy(binaryCode, intToBinaryRes);
        insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, binaryCode, &parsedLine->operands[i]);
        */
        *DC = *DC + 1;
    }

    /* Null byte at the end of the string */
    /* Check if BINARY_CODE_LEN or BINARY_CODE_LEN -1 */
    /*insertToBinaryCodesTable(binaryCodesTable, *DC, parsedLine, convertIntToBinary(0, BINARY_CODE_LEN), &parsedLine->operands[i]);*/
    *DC = *DC + 1;     /*for '/0'*/
    return error_flag; /* 0 -> SUCCESS*/
}

int handleExternDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head)
{
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    if (parsedLine->label != NULL)
    {
        error_flag = ERROR_EXTERN_NOT_VALID;
        return error_flag;
    }
    token = strtok(parsedLine->operands, ","); /*in .extern the labels will be the operands.*/
    while (token != NULL)
    {                                   /*if number of labels is bigger then one*/
        token = trim_whitespace(token); /*returns char pointer after whitespaces and ends before whitespaces.*/
        if (strlen(token) == 0)
        { /*symbol empty, can happen between two ','*/
            error_flag = ERROR_SYMBOL_SHORT;
            return error_flag;
        }
        error_flag = add_symbol_to_table(symbolTable, token, TYPE_EXTERN, 0, macro_head); /*extern is type 4. checks inside if the symbol is in valid name*/
        if (error_flag != 0)
        { /*if some label not valid.*/
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}

int handleEntryDirective(AssemblyLine *parsedLine, SymbolTable *symbolTable, BinaryLine **binary_table, struct macros *macro_head)
{
    /* TO DO - add if there is space between operands*/
    char *token = NULL;
    ErrorCode error_flag = 0; /* Assume success */
    if (parsedLine->label != NULL)
    {
        error_flag = ERROR_ENTRY_NOT_VALID;
        return error_flag;
    }
    token = strtok(parsedLine->operands, ","); /*in .entry the labels will be the operands.*/
    while (token != NULL)
    {
        token = trim_whitespace(token); /*returns char pointer after whitespaces and ends before whitespaces.*/
        if (strlen(token) == 0)
        { /*symbol empty, can happen between two ','*/
            error_flag = ERROR_SYMBOL_SHORT;
            return error_flag;
        }
        error_flag = add_symbol_to_table(symbolTable, token, TYPE_ENTRY, 0, macro_head); /*extern is type 3. checks inside if the symbol is in valid name*/
        if (error_flag != 0)
        { /*if some label not valid.*/
            return error_flag;
        }
        token = strtok(NULL, ",");
    }
    return error_flag; /*0 -> SUCCESS*/
}