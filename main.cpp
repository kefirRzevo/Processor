#include "include/lexer.h"
#include "include/parser.h"
#include "include/cpu.h"

int main()
{
    array  idents_arr = {};
    array  tokens_arr = {};
    
    token* tokens = tokenize("asmcode", &idents_arr, &tokens_arr);
    create_binfile("binary", tokens);
    processor("binary");

    //array_dtor(&idents_arr, 1);
    //array_dtor(&tokens_arr, 0);
    return 0;
}