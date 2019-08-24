/*
 * @File
 * This file contains function pointer (functors)
 * The functors are used to define the generic function call
 * the command handlers to access the symbols in the table
 */


#ifndef _GENERIC_FUNCTION_H
#define _GENERIC_FUNCTION_H

#include "iostream"
#include "stdlib.h"
#include "vector"
#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdint.h"
#include "tgmath.h"
#include "string.h"
#include "sstream"

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0



//define aliases for existing data types for ease of accessing them from the debugger
typedef float float_d;
typedef long long_d; //long type not required since we already have uint32
typedef double double_d;

using namespace std;

/*
#define typeofvar(x) _Generic((x), \
                    char:"char", \
                    int:"int",\
                    short int:"short int",\
                    long int:"long int",\
                    double:"double",\
                    float:"float",\
                    bool:"bool"\
)(x)
 */

struct p_used
{
        bool first = false;
        bool second = false;
        bool third = false;
        bool fourth = false;
};

/**typedefs for the possible types of functions for dynamically handling the functions**/

//to help remember what type was used in the union
enum union_type_used
{
    CHAR, //can use long
    UINT32_T, //can use long
    FLOAT,
    DOUBLE,
    SIGNED_INT, //can use long
    USR_DEFINED
};

struct data_types_struct
{
        enum union_type_used type_used;

        union data_types
        {
                int8_t c;
                uint32_t i;
                float f;
                double d;
                signed int si;
        }types_union;
};

struct data_type_used
{
    bool CHAR1,
    CHAR2,
    CHAR3,
    CHAR4,

    UINT32_T1,
    UINT32_T2,
    UINT32_T3,
    UINT32_T4,

    FLOAT1,
    FLOAT2,
    FLOAT3,
    FLOAT4,

    DOUBLE1,
    DOUBLE2,
    DOUBLE3,
    DOUBLE4,

    SIGNED_INT1,
    SIGNED_INT2,
    SIGNED_INT3,
    SIGNED_INT4;
};


struct data_type_used_struct
{
        char char1;
        bool char1_1;

        char char2;
        bool char2_1;

        char char3;
        bool char3_1;

        char char4;
        bool char4_1;

        uint32_t uint1;
        bool uint1_1;

        uint32_t uint2;
        bool uint2_1;

        uint32_t uint3;
        bool uint3_1;

        uint32_t uint4;
        bool uint4_1;

        float float1;
        bool float1_1;

        float float2;
        bool float2_1;

        float float3;
        bool float3_1;

        float float4;
        bool float4_1;

        double double1;
        bool double1_1;

        double double2;
        bool double2_1;

        double double3;
        bool double3_1;

        double double4;
        bool double4_1;

        int int1;
        bool int1_1;

        int int2;
        bool int2_1;

        int int3;
        bool int3_1;

        int int4;
        bool int4_1;
};


struct order_of_params
{
        int ch;
        int uint32;
        int fl;
        int dou;
        int i;
};


class VoidGenericFunction
{
    public:
        VoidGenericFunction(){};
        virtual ~VoidGenericFunction(){};
        virtual void operator()(uint32_t addr) = 0;
};

//Generic function handler for four int params and void return type
class IntGenericFunction
{
    public:
        IntGenericFunction(){};
        virtual ~IntGenericFunction(){};
        virtual void operator()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4,
                uint32_t n_param_needed) = 0;
};

class IntReturnGenericFunction
{
    public:
        IntReturnGenericFunction(){};
        virtual ~IntReturnGenericFunction(){};
        virtual uint32_t operator()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4,
                uint32_t n_param_needed) = 0;
};

class PrimitiveGenericFunction
{
    public:
        PrimitiveGenericFunction();
        virtual ~PrimitiveGenericFunction(){};

        //create a typedef that would accept float and double
        virtual void operator()(uint32_t addr) = 0;
        virtual void wrap_buffer(char*);
        virtual vector<data_types_struct> unwrap_buffer(std::string str);

    private:
        //buffer that holds the params instead of trying to guess what the user would need
        char *param_buf;
        //float, double, float
        //param_buf = ();
};


/***********************Classes inheriting the abstract classes*****************************/
//A generic void function handler
class VoidFunctionHandler:public VoidGenericFunction
{
    public:
        VoidFunctionHandler(){};
        void operator()(uint32_t addr);
};

class IntFunctionHandler:public IntGenericFunction
{
    public:
        IntFunctionHandler(){};

        //n_param_needed is here to check how many params we need for the
        //actual function to be executed
        void operator()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
                uint32_t param4, uint32_t n_param_neeeded);
};

class IntReturnFunctionHandler : public IntReturnGenericFunction
{
    public:
        IntReturnFunctionHandler(){};

        uint32_t operator()(uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
                uint32_t param4, uint32_t n_param_neeeded);
};

class PrimitiveGenericHandler: public PrimitiveGenericFunction
{
    public:
        void operator()(uint32_t addr);
        void wrap_buffer(char*);
        vector<data_types_struct> unwrap_buffer(std::string);
        char* get_buffer();
};

void void_generic_func_caller(VoidGenericFunction *gen_func, uint32_t addr);


void void_int_generic_caller(IntGenericFunction *gen_func, uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint8_t n_param_neeeded);

uint32_t int_return_generic_caller(IntReturnGenericFunction *gen_func, uint32_t addr, uint32_t param1, uint32_t param2, uint32_t param3,
        uint32_t param4, uint8_t n_param_neeeded);

void generic_function_handler(uint32_t, std::vector<data_types_struct>);

void gen_test();
#endif
