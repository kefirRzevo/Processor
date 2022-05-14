#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#include "res/array.h"

enum tok_type
{
    CMD  = 0,
    REG  = 1,
    NUM  = 2,
    KW   = 3,
    CH   = 4,
    LAB  = 5,
};

union tok_value
{
    char   command;
    char   reg;
    double number;
    char   keyword;
    char   ch;
    char*  label;
};

struct token
{
    tok_value value;
    size_t    line; 
    tok_type  type; 
};


token* tokenize(const char* file_path, array* idents, array* tokens);


#endif 