#ifndef PROCESSOR_LEXER_H
#define PROCESSOR_LEXER_H


#include "global.h"


struct CommandLexemes
{
   char  *command_name;
   char  *argument_string;
};


size_t lex(char *const p_asmcode, const size_t* const p_asmcode_size, CommandLexemes **pp_command_lexemes);


#endif //PROCESSOR_LEXER_H