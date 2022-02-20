#ifndef PROCESSOR_PARSER_H
#define PROCESSOR_PARSER_H


#include "global.h"
#include "lexer.h"


size_t parse(CommandLexemes *const commands_lexemes, const size_t commands_lexemes_number, Command **const p_commands);


#endif //PROCESSOR_PARSER_H
