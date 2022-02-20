#ifndef PROCESSOR_GLOBAL_H
#define PROCESSOR_GLOBAL_H


#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <assert.h>
#include <string.h>
#include <ctime>
#include "assembler.h"


const size_t DARK_REGISTERS_NUMBER  = 4;
const size_t REGISTERS_NUMBER       = 16;
const double EPSILON                = 0.00000001; 
const size_t RAM_SIZE               = 128;  
const size_t WIDTH                  = 100;
const size_t HEIGHT                 = 60;


enum type_of_argument
{
    is_register     = 1,
    is_number       = 2,
    is_label        = 3,
    is_ram_register = 4,
    is_ram_number   = 5,
    is_nothing      = 6,
    is_char         = 7,
};


enum error_type
{
   problems_with_argument = 1,
   fread_error            = 2,
};


struct Command
{
    char              key;
    type_of_argument  argument_type;    
    double            argument;
};



#define DEF_CMD(cmd_name, cmd_key, arguments_number, code)   \
        CMD_##cmd_name = cmd_key,

enum CommandKey
{
    #include "../cpu/commands.txt"
};

#undef DEF_CMD



#endif //PROCESSOR_GLOBAL_H