#include "generic_function.h"



using namespace std;

void VoidFunctionHandler::operator ()(uint32_t addr)
{
    //execute here
    typedef void(*funcptr)();
    funcptr generic_func;
    generic_func = (funcptr)addr;
    generic_func();
}

void IntFunctionHandler::operator ()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint32_t n_param_neeeded)
{
    if(n_param_neeeded == 1)
    {
        typedef void(*funcptr)(uint32_t);
        funcptr func;
        func = (funcptr)addr;
        func(param1);
    }
    else if(n_param_neeeded == 2)
    {
        typedef void(*funcptr)(uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        func(param1, param2);
    }
    else if(n_param_neeeded == 3)
    {
        typedef void(*funcptr)(uint32_t, uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        func(param1, param2, param3);
    }
    else if(n_param_neeeded == 4)
    {
        typedef void(*funcptr)(uint32_t, uint32_t, uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        func(param1, param2, param3, param4);
    }
}

uint32_t IntReturnFunctionHandler::operator ()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint32_t n_param_neeeded)
{
    if(n_param_neeeded == 1)
    {
        typedef uint32_t (*funcptr)(uint32_t);
        funcptr func;
        func = (funcptr)addr;
        return func(param1);
    }
    else if(n_param_neeeded == 2)
    {
        typedef uint32_t(*funcptr)(uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        return func(param1, param2);
    }
    else if(n_param_neeeded == 3)
    {
        typedef uint32_t(*funcptr)(uint32_t, uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        return func(param1, param2, param3);
    }
    else if(n_param_neeeded == 4)
    {
        typedef uint32_t(*funcptr)(uint32_t, uint32_t, uint32_t, uint32_t);
        funcptr func;
        func = (funcptr)addr;
        return func(param1, param2, param3, param4);
    }

    return -1;
}


void generic_function_handler(uint32_t addr, std::vector<data_types_struct> data_struct)
{
    if(data_struct.empty())
    {
        printf("Empty parameters\n");
        return;
    }

    printf("Passing parameters to function\n");

    asm (
            "mov %%r0, %0;"\
            "mov %%r1, %1;"\
            "mov %%r2, %2;"\
            "mov %%r3, %3;"\
            "BLX %4;"\
            :
            :"r"(data_struct[0].types_union),
             "r"(data_struct[1].types_union),
             "r"(data_struct[2].types_union),
             "r"(data_struct[3].types_union),
             "r"(addr)\
             :"%r0", "%r1", "%r2", "%r3"\
    );

}

void PrimitiveGenericHandler::wrap_buffer(char* buf)
{
    std::vector<uint8_t> buffer;
    char *temp = buf;

    //    va_list param;
    //    va_arg()
}

int string_to_int(string s)
{
    int i = 0;
    std::stringstream stream;
    stream << s;
    stream >> i;
    return i;
}

vector<data_types_struct> PrimitiveGenericHandler::unwrap_buffer(std::string str)
{
    vector<data_types_struct> types_vector;

    if(str.empty())
        return types_vector;


    const char *c = str.c_str();
    int length = strlen(c);
    int index = 0;

    string temp;

    while(index < length)
    {
        if(c[index] != '\n')
        {
            temp += c[index];
        }
        else
        {
            printf("Param string %s\n", temp.c_str());
            const char *ch = temp.c_str();
            string dot = ".";
            size_t found = temp.find(dot);

            if(found != std::string::npos) //float/double
            {
                double d = atof(ch);
                printf("double val %f %d\n", d);
                data_types_struct double_type;
                double_type.type_used = DOUBLE;
                double_type.types_union.d = d;
                types_vector.push_back(double_type);
            }
            else if(temp.length() == 1) //char
            {
                printf("is a char %s\n", ch);

                data_types_struct char_type;
                char_type.type_used = CHAR;
                char_type.types_union.c = temp.at(0);
                types_vector.push_back(char_type);
            }
            else //int
            {
                string minus = "-";
                size_t i_found = temp.find(minus);

                if(i_found != std::string::npos) //signed int
                {
                    data_types_struct sign_int_type;
                    int i = string_to_int(temp);
                    sign_int_type.type_used = SIGNED_INT;
                    sign_int_type.types_union.si = i;
                    types_vector.push_back(sign_int_type);
                }
                else //unsigned int
                {
                    unsigned int i = string_to_int(temp);
                    printf("is an int %d \n", i);
                    data_types_struct i_type;
                    i_type.type_used = UINT32_T;
                    i_type.types_union.i = i;
                    types_vector.push_back(i_type);
                }
            }
            temp.clear();
        }
        index++;
    }

    free(&c);
    return types_vector;
}

void void_generic_func_caller(VoidGenericFunction *gen_func, uint32_t addr)
{
    (*gen_func)(addr);
}


void void_int_generic_caller(IntGenericFunction *gen_func, uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint8_t n_param_neeeded)
{
    (*gen_func)(addr, param1, param2, param3, param4, n_param_neeeded);
}

uint32_t int_return_generic_caller(IntReturnGenericFunction *gen_func, uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint8_t n_param_neeeded)
{
    return (*gen_func)(addr, param1, param2, param3, param4, n_param_neeeded);
}


void var_args_func(uint32_t n_args,...)
{
    va_list args;
    va_start(args, n_args);

    for(int i = 0; i < n_args; i++)
    {
        printf("%d \n", va_arg(args, uint32_t));
    }

    va_end(args);
}
