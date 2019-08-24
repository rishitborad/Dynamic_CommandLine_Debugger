/*
 * @File
 * This file contains the support function declarations and
 * definitions for the symbol tables based debugger tool
 */

#ifndef DEBUGGER_TABLE_PARSE_H_
#define DEBUGGER_TABLE_PARSE_H_

#include "generic_function.h"

#define TOTAL_PARAMETERS 10



extern char __symbol_table_start__;
extern char __symbol_table_end__;

/// enum to number of user inputs including function name and arguments
enum num_of_inputs
{
    args_zero = 0,
    args_one,
    args_two,
    args_three,
    args_four,
    args_five,
};

/// Function to count the number of tokens from the user string
int count_args(char *buffer)
{
    uint args_count = 0;
    uint i;
    char ch;

    for (i = 0; i < strlen(buffer); i ++){
        ch = buffer[i];
        if((isblank(ch)) || (buffer[i] == '\0')){
            args_count += 1;
        }
    }
    return (args_count + 1);
}

/// Function to determine the data type for user input argument values
data_types_struct get_type(char *c_str)
{
    char *found = '\0';
    char dot = '.';

    found = strchr(c_str, dot);

    if(found)
    {
        float f = atof(c_str);
        data_types_struct float_type;
        float_type.types_union.f = f;
        float_type.type_used = FLOAT;
        return float_type;
    }
    else
    {
        char minus = '-';
        char plus = '+';
        char *minus_sign = '\0';
        char *plus_sign = '\0';
        minus_sign = strchr(c_str, minus);
        plus_sign = strchr(c_str, plus);

        if(minus_sign)
        {
            signed int si = atoi(c_str);
            data_types_struct signed_type;
            signed_type.types_union.si = si;
            signed_type.type_used = SIGNED_INT;
            return signed_type;
        }
        else if(plus_sign)
        {
            uint32_t i = atoi(c_str);
            data_types_struct unsigned_type;
            unsigned_type.types_union.i = i;
            unsigned_type.type_used = UINT32_T;
            return unsigned_type;
        }
        else
        {
            signed int si = atoi(c_str);
            data_types_struct signed_type;
            signed_type.types_union.si = si;
            signed_type.type_used = SIGNED_INT;
            return signed_type;
        }
    }
}

bool symbolTable_parser(vector<data_types_struct> data_struct,
                        char * usr_func,
                        uint usr_parameter_count,
                        char * sym_type,
                        uint32_t * execution_addr)
{
    char * symbol_start_address = &__symbol_table_start__;
    char * symbol_end_address = &__symbol_table_end__;

    char *check_char;
    char *check_short;
    char *check_int;
    char *check_long;

    char *check_unsigned_char;
    char *check_unsigned_short;
    char *check_unsigned_int;
    char *check_unsigned_long;

    char *check_float;
    char *check_double;
    char *check_long_double;
    char *curr_char;
    char symbolType;
    char symbolAddr[11]={};
    char symbolName[256]={};
    char symbolSection[6]={};
    char para[TOTAL_PARAMETERS][128]={};

    int num_o_space = 0;
    int curr_variable = 0;
    int curr_parameter = 0;
    int curr_parameter_f = 0;
    int func_found_f = 0;
    int i = 0, j = 0, k = 0;

    enum union_type_used para_type[TOTAL_PARAMETERS];

    char *current_line = symbol_start_address;
    while (current_line < symbol_end_address)
    {
        memset(&symbolType,0,sizeof(symbolType));
        memset(&symbolAddr,0,sizeof(symbolAddr));
        memset(&symbolSection,0,sizeof(symbolSection));
        memset(&symbolName,0,sizeof(symbolName));

        memset(&para[1],0,sizeof(para[1]));
        memset(&para[2],0,sizeof(para[2]));
        memset(&para[3],0,sizeof(para[3]));
        memset(&para[4],0,sizeof(para[4]));

        /*** reset variables to default ***/
        curr_char = current_line;
        num_o_space = 0;
        curr_variable = 1;
        curr_parameter = 0;
        i = 0;

        /**** there are 2 parts of a line, 1) Variable part 2) Parameter part,
            * variable part consist symbolType, symbolName, symbolAddr, symbol section,
            * parameter part consist of para1, para2, para3, para4.
            * String before '(' is a variable part, string between '(' and ')' is a parameter part
            * program knows what part it is in based on curr_variable and curr_parameter flags,
            * when char pointer is in variable part, curr_parameter is 0 or -1,
            * when char pointer is in parameter part, curr_parameter is greater than 0.
        *****/

        /*** Read the line char-by-char ***/
        while(*curr_char != '\n'){
            if(*curr_char == ' '){
                num_o_space++;                              // count spaces, change variable and parameter after 1st space, ignore other spaces
                if(num_o_space == 1){
                    /*** incremets variable in variable part ***/
                    if(curr_parameter == 0){
                        curr_variable++;                    // increase the counter after space to change the variable
                        i = 0;                              // reset the incrementer for char arrays
                    }
                    /*** 4 else if in this block handles spaces between each parameter ***/
                    else
                        para[curr_parameter][i++] = *curr_char;             // fill parameter 1
                    }
                else
                ;
            }
            else if(*curr_char == '('){
                num_o_space = 0;                            // reset space counter, to work with parameters now
                curr_variable = 0;                          // close access to variable manipulation block
                curr_parameter_f = 1;                         // get access to parameter manipulation block
                i = 0;                                      // reset value of i for first parameter
            }
            else if(*curr_char == ')'){
                i = 0;// after ')', compare function name with user_string
                break;
            }
            else if(*curr_char == ','){
                if(curr_parameter != 0){
                    curr_parameter++;                   // increase the parameter counter to change the paramater
                    i = 0;                              // reset i in parameter part
                }
                else
                    ;
            }
            else if(*curr_char != ' '){
                if(curr_parameter_f == 1)
                {
                    curr_parameter = 1;
                    curr_parameter_f = 0;
                }
                num_o_space = 0;                            // reset space, to increment variable/parameter counter
                if(curr_variable != 0){                     // enter if variable block is enable
                    if(curr_variable == 1) {                 // for variable = 1, fill first variable (flag)
                        symbolType = *curr_char;
                        *sym_type = *curr_char;
                    }
                    else if(curr_variable == 2)             // for variable = 2, fill third variable (Address in hex)
                        symbolAddr[i++] = *curr_char;
                    else if(curr_variable == 3)             // for variable = 3, fill second variable (section)
                        symbolSection[i++] = *curr_char;
                    else if(curr_variable == 4)             // for variable = 4, fill fourth variable (symbol name)
                        symbolName[i++] = *curr_char;
                    else
                        ;
                }
                else if(curr_parameter != 0){               // enter if parameter block is enable
                    para[curr_parameter][i++] = *curr_char;
                }
            }
            curr_char++;                                    // increment char pointer of main string
        }

        /*** break for loop when break_loop_f is raised (symbl matches with user string) ***/
        if(0 == strcmp(symbolName,usr_func)){
            printf("Function found\n");
            func_found_f = 1;
            break;
        }

        num_o_space = 0;
        curr_variable = 0;
        curr_parameter = 0;

        /*** If the symbol not found then go to the next line of symbol table ***/
        char * next = strchr(current_line, '\n');

        /*
         * If the current line reaches the end of the symbol table without finding
         * the symbol given from the user function, then the symbol is not present
         * in the symbol table, maybe because, it is not created or it not being used.
         */
        current_line = next + 1;
        if (current_line == symbol_end_address)
        {
            printf("Function not found\n");
            break;
        }
    }

    if(curr_parameter != usr_parameter_count)
    {
        printf("Incorrect number of function parameters\n");
        return false;
    }

   uint32_t address = (uint32_t)strtol(symbolAddr, NULL, 16);
   address += 1;                                                    // The symbol address in the elf is 1 higher than the one in map/lst.
   *execution_addr = address;

    /*** Checks the type of the symbol ***/
    if (curr_parameter != 0 && func_found_f == 1)
    {
        /*** Find the type of parameters ***/
        for(k = 1; k<=curr_parameter; k++){
            check_unsigned_char = strstr(para[k],"unsigned char");
            if(check_unsigned_char != NULL){
                para_type[k] = UINT32_T;
                continue;
            }

            check_char = strstr(para[k],"char");
            if(check_char != NULL){
                para_type[k] = SIGNED_INT;
                continue;
            }

            check_unsigned_short = strstr(para[k],"unsigned short");
            if(check_unsigned_short != NULL){
                para_type[k] = UINT32_T;
                continue;
            }

            check_short = strstr(para[k],"short");
            if(check_short != NULL){
                para_type[k] = SIGNED_INT;
                continue;
            }

            check_unsigned_int = strstr(para[k],"unsigned int");
            if(check_unsigned_int != NULL){
                para_type[k] = UINT32_T;
                continue;
            }

            check_int = strstr(para[k],"int");
            if(check_int != NULL){
                para_type[k] = SIGNED_INT;
                continue;
            }

            check_unsigned_long = strstr(para[k],"unsigned long");
            if(check_unsigned_long != NULL){
                para_type[k] = UINT32_T;
                continue;
            }

            check_long_double = strstr(para[k],"long double");
            if(check_long_double != NULL){
                para_type[k] = DOUBLE;
                continue;
            }

            check_long = strstr(para[k],"long");
            if(check_long != NULL){
                para_type[k] = UINT32_T;
                continue;
            }

            check_float = strstr(para[k],"float");
            if(check_float != NULL){
                para_type[k] = FLOAT;
                continue;
            }

            check_double = strstr(para[k],"double");
            if(check_double != NULL){
                para_type[k] = DOUBLE;
                continue;
            }
            para_type[k] = USR_DEFINED;
        }
    }

    for(j = 1 ; j <= curr_parameter; j++){
        if(data_struct[j-1].type_used != para_type[j])
            return false;
    }
    return true;
}

/// Sample global variables for testing
int     val_int = 345;

/// Sample functions for testing
void foo()
{
    printf("The value of sqr(val_int): %d\n", val_int * val_int);
}

void sqri(int a)
{
    printf("sqr(%d): %d\n", a, (a*a));
}

void sqrf(float a)
{
    printf("sqr(%f): %f\n", a, (a*a));
}

#endif /* DEBUGGER_TABLE_PARSE_H_ */
