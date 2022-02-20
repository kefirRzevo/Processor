#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include "../include/lexer.h"


static size_t num_cmds(char* const p_asmcode, const size_t* const p_asmcode_size)
{
    assert(p_asmcode);
    assert(p_asmcode_size);

    size_t n_commands        = 0;
    size_t n_comment_signs   = 0;
    bool   result_flag       = false;
    size_t n_isalpha         = 0;
    
    for(char* iterator = p_asmcode; iterator < p_asmcode + *p_asmcode_size; iterator++)
    { 
        if(isalpha(*iterator) != 0 && *iterator != ';' && result_flag == false)
            n_isalpha++;

        if(*iterator == ';')
        {
            n_comment_signs++;
            result_flag = true;
        }

        if(*iterator == '\n')
        {
            if(n_isalpha!=0)
                n_commands++;

            result_flag     = false;
            n_isalpha       = 0;
            n_comment_signs = 0; 
        }
    }

    return n_commands;
}


static bool skip_comments(char** iterator, char *const p_asmcode, const size_t* const p_asmcode_size)
{
    if(**iterator == ';')
    {
        while(**iterator != '\n' && *iterator < p_asmcode + *p_asmcode_size)
            (*iterator)++;
        return true;
    }
    return false;
}


static bool check_end_of_cmd(char** iterator, char* argument, size_t* counter_cmds, char *const p_asmcode, const size_t *const p_asmcode_size)
{
    char* temp = &(**iterator);
    if(skip_comments(iterator, p_asmcode, p_asmcode_size))
    {
        *temp = '\0';
        argument  = nullptr;
        (*counter_cmds)++;
        return true;
    }
    else if(**iterator == '\n')
    {
        **iterator = '\0';
        argument  = nullptr;
        (*counter_cmds)++;
        (*iterator)++;
        return true;
    }
    else
    {
        if(isalnum(*(*iterator - 1)) != 0 || *(*iterator - 1) == ':')
        {
            **iterator = '\0';
            (*iterator)++;
        }
        return false;
    }
}


static bool check_end_of_arg(char** iterator, size_t* counter_cmds, char *const p_asmcode, const size_t *const p_asmcode_size)
{
    char* temp = &(**iterator);
    if(skip_comments(iterator, p_asmcode, p_asmcode_size))
    {
        *temp = '\0';
        (*counter_cmds)++;
        return true;
    }
    else if(**iterator == '\n')
    {
        **iterator = '\0';
        (*counter_cmds)++;
        (*iterator)++;
        return true;
    }
    else
    {
        if(isspace(*(*iterator - 1)) == 0)
        {
            **iterator = '\0';
            (*iterator)++;
        }
        return false;
    }
}


static void begin_with_hooks_or_no(char** iterator, char* argument, size_t* counter_cmds, char *const p_asmcode, const size_t *const p_asmcode_size)
{
    if((**iterator == '\'' || **iterator == '[') && *iterator < p_asmcode + *p_asmcode_size)
    {       
        (*iterator)++;

        while(**iterator != '\'' && **iterator != ']' && **iterator != '\n' && (*iterator) < p_asmcode + *p_asmcode_size)
            (*iterator)++;

        if(**iterator != ']' && **iterator != '\'')
        {  
            printf("Command $%s$ at number j$%zu$ hasn't got closing hooks\n", argument, *counter_cmds + 1);
            exit(1);
        }
        
        (*iterator)++;

        if(isspace(**iterator) == 0 && **iterator != ';')
        {  
            printf("Command $%s$ at number j$%zu$ has something after hooks\n", argument, *counter_cmds + 1);
            exit(1);
        }
    }  
    else
    {
        while((isalnum(**iterator) != 0 || **iterator == '.' || **iterator == '-' || **iterator == '_') && (*iterator) < p_asmcode + *p_asmcode_size)
            (*iterator)++;
    }
}


static void skip_spaces_before_comment(char** iterator, char *const p_asmcode, const size_t* const p_asmcode_size)
{
    while(isspace(**iterator) != 0 && **iterator != '\n' && **iterator != ';' && (*iterator) < p_asmcode + *p_asmcode_size)
        (*iterator)++; 
}  


size_t lex(char *const p_asmcode, const size_t* const p_asmcode_size, CommandLexemes **pp_command_lexemes)
{
    assert(p_asmcode);
    assert(p_asmcode_size);
    assert(pp_command_lexemes);

    size_t n_commands                 = num_cmds(p_asmcode, p_asmcode_size);
    CommandLexemes* p_command_lexemes = (CommandLexemes *) calloc(n_commands, sizeof(CommandLexemes));
    assert(p_command_lexemes);
 
    size_t counter_cmds = 0;

    for(char* iterator = p_asmcode; iterator < p_asmcode + *p_asmcode_size && counter_cmds < n_commands;)
    {
        while(isspace(*iterator) != 0 && iterator < p_asmcode + *p_asmcode_size)
            iterator++;

        if(skip_comments(&iterator, p_asmcode, p_asmcode_size))
            continue;

        p_command_lexemes[counter_cmds].command_name = iterator;

        while((isalnum(*iterator) != 0 || *iterator == ':' || *iterator == '_') && iterator < p_asmcode + *p_asmcode_size)
            iterator++;

        if(check_end_of_cmd(&iterator, p_command_lexemes[counter_cmds].argument_string, &counter_cmds, p_asmcode, p_asmcode_size))
            continue;
    
        skip_spaces_before_comment(&iterator, p_asmcode, p_asmcode_size);
        
        if(check_end_of_cmd(&iterator, p_command_lexemes[counter_cmds].argument_string, &counter_cmds, p_asmcode, p_asmcode_size))
            continue;
        p_command_lexemes[counter_cmds].argument_string = iterator;

        begin_with_hooks_or_no(&iterator, p_command_lexemes[counter_cmds].command_name, &counter_cmds, p_asmcode, p_asmcode_size);

        if(check_end_of_arg(&iterator, &counter_cmds, p_asmcode, p_asmcode_size))
            continue;

        skip_spaces_before_comment(&iterator, p_asmcode, p_asmcode_size);

        if(skip_comments(&iterator, p_asmcode, p_asmcode_size))
        {
            counter_cmds++;
            continue;
        }

        if(isalnum(*iterator) != 0)
        {
            printf("Command $%s$ at number $%zu$ has more than one argument\n", p_command_lexemes[counter_cmds].command_name, counter_cmds + 1);
            exit(2);
        }
        counter_cmds++;
        continue;     
    }
    
    *pp_command_lexemes = p_command_lexemes;
    
    return n_commands;
}