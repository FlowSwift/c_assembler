#include <stdio.h>
#include <string.h>

#include "first_pass.h"
#include "line_parser.h"
#include "lines.h"
/*FLOW:
    - read line after line, count number of line -
    - parse the line to 3 sections - label(optional), instucrion, operandes
    - for every parsed line, see if it is allowed by a specific function
        - label:
            - is legal
            - if directive line - check if its legal
                - parse operandes accordinly - if string if ...
            - up ID
            - if everything legal - check if already in symbol table (if yes, ERROR), if not - ADD by IC,
                continue, if legal by checking (OperandeCheck).
            - parse operandes.
                if operand is also label ?
            - check if
            - write in Binary structure into Binary list
        - not label -
            - if start with . -> ? (i think end reading)
            - if regular opcode:
                - check how many operandes to receive
                - find number represenation of Opcode
                - parse operands by ',' - if number as many as supposed to be - else ERROR
                    - check type of miun - see if its allowed for this operand
                    - make switch case on what to do binary for every case and edge case 3
                - if operand register - make binary
                - if operand is LABEL - write name only (?)
*/

const Opcode OPCODES[] = {
    {0, "mov", 2},
    {1, "cmp", 2},
    {2, "add", 2},
    {3, "sub", 2},
    {4, "lea", 1},
    {5, "clr", 1},
    {6, "not", 2},
    {7, "inc", 1},
    {8, "dec", 1},
    {9, "jmp", 1},
    {10, "bne", 1},
    {11, "red", 1},
    {12, "prn", 1},
    {13, "jsr", 1},
    {14, "rts", 0},
    {15, "stop", 0}
};

int firstPass(char *file_name, SymbolTable *symbolTable, int *IC, int *DC)
{
    char line[MAX_LINE_LENGTH];
    FILE *amfile = fopen(file_name, "r");
    AssemblyLine parsedLine;
    /* SymbolNode *current; */
    int lineNumber = 0;
    /* errorCode = SUCCESS; */
    /* FILE *file = NULL; */
    /* File file; */
    while (fgets(line, MAX_LINE_LENGTH + 1, amfile) != NULL)
    {
        lineNumber++;
        if (line[strlen(line) - 1] != '\n')
        {
            /* IMPROVE ERRORS*/
        }
        parsedLine = parseAssemblyLine(line);
        printf("Parsed Line:\n");
        printAssemblyLine(&parsedLine);
        if(parsedLine.label!=NULL){
            /*if - call function to check if already defined*/
        }
        if(is_directive(&parsedLine)){
            check_directive(&parsedLine);
        }
        else{
            is_valid_instruction(&parsedLine);
        }
        /* make all the checks if allowed */
        /*if (ParsedLine->label!=NULL){ }*/
        /* all the checks if legal will be here */
        freeAssemblyLine(&parsedLine);
    }
    return 0;
}

int is_directive(AssemblyLine* parsedLine)
{
    /* Check if the first character of the instruction is a dot, as should be in directive ('.')*/
    return (parsedLine->instruction[0] == '.') ? 1 : 0;
}

int countOccurrences(const char *str, char ch) {
    int count = 0;
    while (*str) {
        if (*str == ch) {
            count++;
        }
        str++;
    }
    return count;
}

int check_type(Operand *operand){
    if (operand->value[0] == '\0' or operand->value[0] == NULL) {
        operand->type = -1;
        return 1;
    }
    switch (operand->value[0]) 
    {
        case '#':
            /*Remove '#' and check if the remaining part is a valid integer*/
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            if (is_valid_integer(operand->value)) {
                operand->type = 0;
            } 
            else {
                return 0;
                /* IMPROVE ERRORS*/
            }
            operand->value[strlen(operand->value) - 1] = '\0';
            break;
        case '*':
            memmove(operand->value, operand->value + 1, strlen(operand->value)-1);
            if (strlen(operand->value) == 2 && operand->value[0] == 'r'&& operand->value[1] >= '0' && operand->value[1] <= '7') {
                operand->value[strlen(operand->value) - 1] = '\0';
                operand->type = 0;
                break;
            }
            elsel{
                return 0;
                /* IMPROVE ERRORS*/
            }
        case 'r':
            if (strlen(operand->value) == 2 && operand->value[1] >= '0' && operand->value[1] <= '7') {
                operand->type = 3;
                break;
            }
            else {
                /* IMPROVE ERRORS*/
                /*supposed to be register but not */
                return 0;
            }
        default:
            operand->type = 1; /* Default is it's label, will be checked in 2 pass */
            break;
    }
    return 1;
}

int get_opcode_operands(AssemblyLine* parsedLine){
    /* gets allowed operands of insturction*/
    int i =0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(parsedLine->instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].numOfOperands;
            /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int get_opcode_code(AssemblyLine* parsedLine){
    /* gets opcode number of insturction*/
    int i =0;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(parsedLine->instruction, OPCODES[i].name) == 0) {
            return OPCODES[i].code;
            /*Instruction found in OPCODES*/
        }
    }
    return -1; /*Instruction not found in OPCODES*/
}

int is_valid_integer(char *operand) {
    if (operand == NULL || *operand == '\0') {
        return 0; /*Operand is null or empty*/
    }
    /*Ensure the first character is a digit or '-'*/
    if (!isdigit(*operand) && *operand != '-') {
        return 0; /*First character is not valid*/
    }
    /*Ensure the rest of the characters are digits*/
    operand++; /*Move past the first character*/
    while (*operand != '\0') {
        if (!isdigit(*operand)) {
            return 0; /*Found a non-digit character*/
        }
        operand++;
    }
    return 1; /*Operand is a valid integer*/
}

int operand_parser(AssemblyLine* parsedLine){
    char *ptr_in_line = NULL;
    char *start = NULL;
    ptr_in_line = parsedLine->operands;
    Operand *temp_srcOperand = NULL, *temp_destOperand = NULL;
    int operandCount = 0;
    int num_operands = 0;
    temp_srcOperand  = (Operand *)malloc(sizeof(Operand));
    temp_destOperand  = (Operand *)malloc(sizeof(Operand));
    char *operandValue;
    int operandLen = 0;
    num_operands = get_opcode_operands(parsedLine); /*got how many allowed*/
    if(num_operands == -1){
        /* IMPROVE ERRORS*/
        /*insturction not found*/
        free(temp_srcOperand);
        free(temp_destOperand );
        return 0;
    }
    while (*ptr_in_line != '\0' && operandCount < num_operands) {
        while (*ptr_in_line != '\0' && isspace(*ptr_in_line)) ptr_in_line++; /* Skip whitespace */
        start = ptr_in_line;
        operandLen = 0;
        while (*ptr_in_line != '\0' && *ptr_in_line != ',') { /*after whitespaces*/
            /*go until first ',' the syntax of the value will be checked later*/
            ptr_in_line++;
            operandLen++;
        }
        *operandValue = (char *)malloc(operandLen + 1);
        strncpy(operandValue, start, operandLen);
        operandValue[operandLen] = '\0';
        if (num_operands == 2 && operandCount == 0) {
            temp_srcOperand->value = operandValue;
            if (!check_type(temp_srcOperand)) { /*type of miun will be inserted*/
                /* IMPROVE ERRORS */
                free(temp_srcOperand->value);
                free(temp_destOperand->value);
                free(temp_destOperand);
                free(temp_srcOperand);
                return 0;
            }
        }
        else if (num_operands == 1 || (num_operands == 2 && operandCount == 1)){
            temp_destOperand->value = operandValue; /*if there is only one it is assigned to dest, if teo and this is seconf opperand also dest.*/
            if (!check_type(temp_destOperand)) {/*type of miun will be inserted*/
                /* IMPROVE ERRORS */
                free(temp_srcOperand->value);
                free(temp_destOperand->value);
                free(temp_destOperand);
                free(temp_srcOperand);
                return 0;
            }
        }
        else continue;
        operandCount++;
        if (*ptr_in_line == ',') ptr_in_line++; /* Skip comma */
    }
    /* Check for extra characters after the last operand (up to 2), if num_operands == 0 will also arrive here and check*/
    while (*ptr_in_line != '\0') {
        if (!isspace(*ptr_in_line)) {
            /*there are chars after last operand or operands when needed to be 0*/
            /* IMPROVE ERRORS */
            if (temp_srcOperand->value) free(temp_srcOperand->value);
            if (temp_destOperand->value) free(temp_destOperand->value);
            free(temp_srcOperand);
            free(temp_destOperand);
            return 0;
        }
        ptr_in_line++;
    }
    if(num_operands!=operandCount){
        /* IMPROVE ERRORS*/
        /*too many operands*/
        if (temp_srcOperand->value) free(temp_srcOperand->value);
        if (temp_destOperand->value) free(temp_destOperand->value);
        free(temp_srcOperand);
        free(temp_destOperand);
        return 0;
    }
    /* Assign operands to parsedLine */
    if (num_operands == 1) {
        temp_srcOperand->type  = -1;
        temp_srcOperand->value = '\0';
        parsedLine->destOperand = temp_destOperand;
    }
    else if (operandCount == 0 && parsedLine->operands != NULL){
            /* IMROVE ERRORS*/
            /*chars but shouldnt be */
            free(temp_srcOperand);
            free(temp_destOperand);
            return 0;
    } 
    else if (num_operands == 0 && parsedLine->operands == NULL) {
        srcOperand->type  = -1;
        srcOperand->value = '\0';
        temp_destOperand->type  = -1;
        temp_destOperand->value = '\0';
    }
    else{
        parsedLine->srcOperand = temp_srcOperand;
        parsedLine->destOperand = temp_destOperand;
    }
    int opcode_code = -1;
    int type_miun = -1;
    int type_miun_src = -1;
    int type_miun_dest = -1;
    opcode_code = get_opcode_address(parsedLine);
    type_miun_src = parsedLine->srcOperand->type;
    type_miun_dest = parsedLine->destOperand->type;
    switch (opcode_code){
        /*mov (0), add(2), sub(3), - have {0,1,2,3) types allowed for src*/
        case -1:
            /*cannot find insturction*/
            /*IMPROVE ERRORS*/
            break;
        case 0:
        case 2:
        case 3:
            if (!(parsedLine->srcOperand->type >= 0 && parsedLine->srcOperand->type <=3) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 1:
            /*cmp(1)*/
            if (!(parsedLine->srcOperand->type >= 0 && parsedLine->srcOperand->type <=3) && (parsedLine->destOperand->type >= 0 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 4:
            /*lea(4)*/
            if (!(parsedLine->srcOperand->type == 1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 11:
            /*clr(5), not(6), inc(7), dec(8), red(11)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 9:
        case 10:
        case 13:
            /*jmp (9), bne(10), jsr(1)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 1 && parsedLine->destOperand->type <=2 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
            break;
        case 12:
            /*prn(12)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type >= 0 && parsedLine->destOperand->type <=3 ))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
        case 14:
        case 15:
            /*rts(14), stop(15)*/
            if (!(parsedLine->srcOperand->type == -1) && (parsedLine->destOperand->type == -1))
            {
                /* IMPROVE ERRORS*/
                /*type of operand not aligned to type of opcode*/
            }
        default:
            break;
    }
    /*the instructions mathed the operands succefully and all was allocated in parsedLine*/
    return 1;
}

int is_valid_instruction(AssemblyLine* parsedLine){
    int did_parse = 0;
    did_parse = operand_parser(parsedLine);
    if(did_parse){
        /*IMPROVE ERRORS*/
        /*couldnt parse operands*/
    }
    /*if it parsed -> all operands are as should be for relevent opcode*/
    return 1;
}