#ifndef PROCESSOR_PARSER_H
#define PROCESSOR_PARSER_H


#include "lexer.h"

const size_t INIT_N_CMDS = 100;
const double EPSILON     = 0.000001;

enum type_of_argument
{
    is_register     = 1,
    is_number       = 2,
    is_ram_register = 3,
    is_char         = 4,
    is_nothing      = 5,
    is_label        = 6,
};

struct Command
{
    char              key;
    type_of_argument  argument_type;    
    double            argument;
};

struct labeled_cmd
{
    char*   label;
    int  to;
    int  from;
};

bool create_binfile(const char* binfile_path, token* tokens);


#endif //PROCESSOR_PARSER_H
