#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "../include/parser.h"


struct Label
{
    char*   name;
    size_t  to;
};


static bool _is_register(char* const argument_string, double* num_reg)
{
    assert(argument_string);
    assert(num_reg);

    char* reg_string = argument_string;
    if(strcmp(reg_string, "rax") == 0)
    {
        *num_reg = 0;
        return true;
    }
    else if(strcmp(reg_string, "rbx") == 0)
    {
        *num_reg = 1;
        return true;
    }
    else if(strcmp(reg_string, "rcx") == 0)
    {
        *num_reg = 2;
        return true;
    }
    else if(strcmp(reg_string, "rdx") == 0)
    {
        *num_reg = 3;
        return true;
    }
    else if(strcmp(reg_string, "rbp") == 0)
    {
        *num_reg = 4;
        return true;
    }
    else if(strcmp(reg_string, "rsi") == 0)
    {
        *num_reg = 5;
        return true;
    }
    else if(strcmp(reg_string, "rdi") == 0)
    {
        *num_reg = 6;
        return true;
    }
    else if(strcmp(reg_string, "rsp") == 0)
    {
        *num_reg = 7;
        return true;
    }
    else if(strcmp(reg_string, "8r") == 0)
    {
        *num_reg = 8;
        return true;
    }
    else if(strcmp(reg_string, "9r") == 0)
    {
        *num_reg = 9;
        return true;
    }
    else if(strcmp(reg_string, "01r") == 0)
    {
        *num_reg = 10;
        return true;
    }
    else if(strcmp(reg_string, "11r") == 0)
    {
        *num_reg = 11;
        return true;
    }
    else if(strcmp(reg_string, "21r") == 0)
    {
        *num_reg = 12;
        return true;
    }
    else if(strcmp(reg_string, "31r") == 0)
    {
        *num_reg = 13;
        return true;
    }
    else if(strcmp(reg_string, "41") == 0)
    {
        *num_reg = 14;
        return true;
    }
    else if(strcmp(reg_string, "51r") == 0)
    {
        *num_reg = 15;
        return true;
    }
    else 
        return false;
}


static void rounding(double* x)
{
    if(fabs(*x) < EPSILON)
            *x = 0;  
}


static bool _is_number(const char* const argument_string, double* x)
{
    assert(argument_string);
    assert(x);

    int pos = 0;

    if(sscanf(argument_string, "%lf%n", x, &pos) > 0)
    {
        if(strlen(argument_string) > (size_t) pos)
            return false;

        rounding(x);

        return true;
    }
    return false;
}


static bool _is_label(const CommandLexemes* const p_command_lexem)
{
    size_t label_length = strlen(p_command_lexem->command_name);

    return ((p_command_lexem->command_name[label_length - 1] == ':') && \
    p_command_lexem->argument_string == nullptr && label_length > 1);
}


static bool _is_ram_register(const char* const argument_string, double* x)
{
    assert(argument_string);
    assert(x);

    size_t argument_length = strlen(argument_string);

    if(argument_length <= 2)
        return false;

    char* temp_string     = (char *)calloc(argument_length + 1, sizeof(char));  
    assert(temp_string);

    strcpy(temp_string, argument_string);
    *(temp_string + argument_length - 1) = '\0'; 
    temp_string++;

    if(_is_register(temp_string, x))
    {
        temp_string--;
        free(temp_string);
        return true;
    }

    temp_string--;
    free(temp_string);
    return false;
}


static bool _is_ram_number(const char* const argument_string, double* x)
{
    assert(argument_string);
    assert(x);

    size_t argument_length = strlen(argument_string);
    
    if(argument_length <= 2)
        return false;

    char* temp_string = (char *)calloc(argument_length + 1, sizeof(char));  
    assert(temp_string);

    strcpy(temp_string, argument_string);
    *(temp_string + argument_length - 1) = '\0'; 
    temp_string++;

    if(_is_number(temp_string, x))
    {
        temp_string--;
        free(temp_string);
        return true;
    }
    
    temp_string--;
    free(temp_string);
    return false;
}


static bool _is_char(const char* const argument_string, double* x)
{
    assert(argument_string);
    assert(x);

    size_t argument_length = strlen(argument_string);
    
    if(argument_length != 3 || argument_string[0] != '\'' || argument_string[2] != '\'')
        return false;

    char* temp_string = (char *)calloc(4, sizeof(char));  
    assert(temp_string);

    strcpy(temp_string, argument_string);
    *(temp_string + argument_length - 1) = '\0'; 
    temp_string++;

    char c = 0;
    if(sscanf(temp_string, "%c", &c) > 0)
    {
        *x = c;
        temp_string--;
        free(temp_string);
        return true;
    }
    
    temp_string--;
    free(temp_string);
    return false;
}


static Label* creating_labels_array(const CommandLexemes *const p_command_lexemes, const size_t command_lexemes_number, size_t* n_labels)
{
    Label* labels         = nullptr;

    size_t counter_labels = 0;


    for(size_t i = 0; i < command_lexemes_number; i++)
    {
        if(_is_label(&p_command_lexemes[i]))
        {
            if(i == command_lexemes_number - 1)
            {
                printf("%s Code can't end with label!!!\n", p_command_lexemes[i].command_name);
                exit(0x405);
            }

            counter_labels++;
            labels = (Label *)realloc(labels, counter_labels * sizeof(Label));
            assert(labels);

            labels[counter_labels - 1].name = p_command_lexemes[i].command_name;
            labels[counter_labels - 1].to   = i + 2 - counter_labels;
        }
    }

    if(counter_labels > 1)
        for(size_t i = 0; i < counter_labels; i++)
            for(size_t j = 0; j < counter_labels; j++)
                if(strcmp(labels[i].name, labels[j].name) == 0 && i!= j)
                {
                    printf("%s This label is used more than one time!!!\n", labels[i].name);
                    exit(0x406);
                }    

    *n_labels = counter_labels;
    return labels;
}

static bool searching_for_label(const Label* const labels, const CommandLexemes *const p_command_lexem, const size_t n_labels, Command *const p_command)
{
    if(labels == nullptr)
        return false;

    for(size_t i = 0; i < n_labels; i++)
    {
        if(strncmp(labels[i].name, p_command_lexem->argument_string, strlen(labels[i].name) - 1) == 0)
        {
            p_command->argument = labels[i].to;
            return true;
        }
    } 

    return false;
}


static void counting_defined_commands(size_t *const n_defined_commands)
{
    #define DEF_CMD(cmd_name, cmd_key, arguments_number, code) \
        (*n_defined_commands)++;

    #include "../cpu/commands.txt"

    #undef DEF_CMD
}


static bool write_argument(Command *const p_commands, CommandLexemes *const p_command_lexemes, const Label *const labels, size_t n_labels)
{
    if(p_command_lexemes->argument_string == nullptr)                                                            
    {                                                                                                           
        p_commands->argument_type = is_nothing;                                                               
        return true;                                                                                               
    }                                                                                                           
    else if(_is_number(p_command_lexemes->argument_string, &p_commands->argument))                          
    {                                                                                                           
        p_commands->argument_type = is_number;                                                                
        return true;                                                                                               
    }                                                                                                           
    else if(_is_ram_number(p_command_lexemes->argument_string, &p_commands->argument))                      
    {                                                                                                           
        p_commands->argument_type = is_ram_number;                                                            
        return true;                                                                                               
    }                                                                                                           
    else if(_is_ram_register(p_command_lexemes->argument_string, &p_commands->argument))                    
    {                                                                                                           
        p_commands->argument_type = is_ram_register;                                                          
        return true;                                                                                               
    }                                                                                                           
    else if(_is_register(p_command_lexemes->argument_string, &p_commands->argument))                        
    {                                                                                                           
        p_commands->argument_type = is_register;                                                              
        return true;                                                                                               
    }                                                                                                           
    else if(_is_char(p_command_lexemes->argument_string, &p_commands->argument))                            
    {                                                                                                           
        p_commands->argument_type = is_char;                                                                  
        return true;                                                                                              
    }                                                                                                           
    else if(searching_for_label(labels, p_command_lexemes, n_labels, p_commands))                       
    {                                                                                                           
        p_commands->argument_type = is_label;                                                                 
        return true;                                                                                               
    } 
    return false;                                                                                                          
}


size_t parse(CommandLexemes *const p_command_lexemes, const size_t command_lexemes_number, Command **const pp_commands)
{
    assert(p_command_lexemes);
    assert(pp_commands);

    Command* p_commands = (Command *) calloc(command_lexemes_number, sizeof(Command));
    assert(p_commands);

    size_t n_labels           = 0;
    size_t n_defined_commands = 0;
    size_t n_commands         = 0;

    Label* labels   = creating_labels_array(p_command_lexemes, command_lexemes_number, &n_labels);

    counting_defined_commands(&n_defined_commands);
    
    for(size_t i = 0, j = 0; i < command_lexemes_number; i++)
    {
        size_t num_def = 0;
        //printf("\n@com:|%s|arg:|%s|type:|%d|\n", p_command_lexemes[i].command_name, p_command_lexemes[i].argument_string, p_commands[i].argument_type);
        #define DEF_CMD(cmd_name, cmd_key, arguments_number, code)                                                              \
        num_def++;                                                                                                              \
                                                                                                                                \
        if (strcmp(p_command_lexemes[i].command_name, #cmd_name) == 0)                                                          \
        {                                                                                                                       \
            p_commands[j].key = cmd_key;                                                                                        \
                                                                                                                                \
            if(arguments_number == 0 && p_command_lexemes[i].argument_string != nullptr)                                        \
            {                                                                                                                   \
                printf("Command $%s$ hasn't got an argument\n", #cmd_name);                                                     \
                exit(0x408);                                                                                                    \
            }                                                                                                                   \
            else if(arguments_number == 1 && p_command_lexemes[i].argument_string == nullptr)                                   \
            {                                                                                                                   \
                printf("Command $%s$ has got an argument\n", #cmd_name);                                                        \
                exit(0x408);                                                                                                    \
            }                                                                                                                   \
            else if(write_argument(&p_commands[j], &p_command_lexemes[i], labels, n_labels))                                    \
            {                                                                                                                   \
                j++;                                                                                                            \
                n_commands++;                                                                                                   \
                continue;                                                                                                       \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                printf("Argument $%s$ of command $%s$ isn't recognized\n", p_command_lexemes[i].argument_string, #cmd_name);    \
                exit(0x407);                                                                                                    \
            }                                                                                                                   \
            continue;                                                                                                           \
        }                                                                                                                       \
        else if(_is_label(&p_command_lexemes[i]))                                                                               \
        {                                                                                                                       \
            continue;                                                                                                           \
        }                                                                                                                       \
        else if(num_def == n_defined_commands)                                                                                  \
        {                                                                                                                       \
            printf("Command $%s$ isn't recognized\n", p_command_lexemes[i].command_name);                                       \
            exit(0x408);                                                                                                        \
        } 

        #include "../cpu/commands.txt"

        #undef DEF_CMD
    }

    free(labels);

    *pp_commands = p_commands;
    
    return n_commands;
}