#ifndef LOGS_ASM_H
#define LOGS_ASM_H


#include "global.h"
#include "lexer.h"


void assembler_logfile (const Command *const p_commands, const CommandLexemes *const p_command_lexemes, const size_t command_lexemes_number);


#endif //LOGS_ASM_H