#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "../include/parser.h"

#define $$ fprintf(stderr, "%d\t%zu\n", __LINE__, iterator);

static size_t iterator  = 0;
static array    labels  = {};
static Command*   cmds  = nullptr;
static size_t   n_cmds  = 0;
static token* p_tokens  = nullptr;
static size_t cmds_size = INIT_N_CMDS*10;

static bool GetG();
static bool GetE();
static bool GetT();
static bool GetP();
static bool GetU();

static char*  is_lab    (size_t position);
static bool   is_num    (size_t position);
static bool   is_reg    (size_t position);
static bool   is_command(size_t position);
static char   is_ch     (size_t position);
static char   is_keyword(size_t position, char name);

static const char*  get_cmd_string(char type);
static const char*  get_reg_string(char type);
static char  get_cmd_num(const char* command);
static char  get_reg_num(const char* registry);
static bool  print_error(const char* waited);
static int   add_cmd(char key, type_of_argument type, double argument);

static int fill_labels();
static int commands_dump(const char* dump_file_path);
static int write_cmds(const char* binfile_path, const Command* p_commands, const size_t command_number);

static const char* get_cmd_string(char type)
{
    #define DEF_CMD(name, cmd, num_arg, code)   \
                        else if (cmd == type)   \
                            return #name;                      

                    if (0) {} 
    #include "../cpu/commands.txt"
    #undef DEF_CMD

    return nullptr;
}

static const char*  get_reg_string(char type)
{
    #define DEF_REG(name, reg)                  \
                        else if (reg == type)   \
                            return #name;                      

                    if (0) {} 
    #include "../cpu/registers.txt"
    #undef DEF_REG

    return nullptr;
}

static char get_cmd_num(const char* command)
{
    #define DEF_CMD(name, cmd, num_arg, code)                               \
                        else if (!strncmp(#name, command, sizeof(#name)))   \
                            return cmd;                      

                    if (0) {} 
    #include "../cpu/commands.txt"
    #undef DEF_CMD

    return 0;
}

static char get_reg_num(const char* registry)
{
    #define DEF_REG(name, reg)                                              \
                        else if (!strncmp(#name, registry, sizeof(#name)))  \
                            return reg;                      

                    if (0) {} 
    #include "../cpu/registers.txt"
    #undef DEF_REG

    return 0;
}


static bool print_error(const char* waited)
{
    fprintf(stderr, "%d\tMistake at line %zu: waited \"%s\", in fact ", __LINE__, p_tokens[iterator].line, waited);  
    switch(p_tokens[iterator].type)                                                                             
    {                                                                                                           
        case CMD:   
            fprintf(stderr, "COMMAND \"%s\".\n", get_cmd_string(p_tokens[iterator].value.command)); 
            break;                                                                                  
        case REG:    
            fprintf(stderr, "REGISTER \"%s\".\n", get_reg_string(p_tokens[iterator].value.reg));             
            break;                                                                                  
        case NUM:    
            fprintf(stderr, "NUMBER \"%lg\".\n", p_tokens[iterator].value.number);                  
            break;   
        case KW:    
            fprintf(stderr, "KEYWORD \"%c\".\n", p_tokens[iterator].value.keyword);                  
            break;  
        case CH:    
            fprintf(stderr, "CHAR \"%c\".\n", p_tokens[iterator].value.ch);                  
            break;  
        case LAB:    
            fprintf(stderr, "LABEL \"%s\".\n", p_tokens[iterator].value.label);                  
            break;                                                                                 
        default:        
            fprintf(stderr, "unpredictable error.\n");
    }
    return nullptr;
}

static int add_cmd(char key, type_of_argument type, double argument)
{
    /*if(n_cmds == cmds_size)
    {
        commands_dump("logfiles/cmds_dump1.txt");
        cmds_size         = cmds_size * 2;
        //Command* new_cmds = (Command* )realloc(cmds, cmds_size);
        Command* new_cmds = (Command* )calloc(cmds_size, sizeof(Command));
        memcpy(new_cmds, cmds, cmds_size);
        cmds = new_cmds;
        if(!new_cmds)
            return 0;
        cmds = new_cmds;
        commands_dump("logfiles/cmds_dump2.txt");
    }*/

     #define DEF_CMD(name, cmd, num_arg, code)                                                      \
                    else if (cmd == key && num_arg == 0 && type != is_nothing)                      \
                    {                                                                               \
                        fprintf(stderr, "%d\tCommand %s hasn't got arguments.", __LINE__, #name);   \
                        return 0;                                                                   \
                    }               

                    if (0) {} 
    #include "../cpu/commands.txt"
    #undef DEF_CMD

    cmds[n_cmds].key           = key;
    cmds[n_cmds].argument_type = type;
    cmds[n_cmds].argument      = argument;
    
    n_cmds++;
    return 1;
}


static char* is_lab(size_t position)
{
    if(p_tokens[position].type == LAB)
        return p_tokens[position].value.label;
    return nullptr;
}

static bool is_num(size_t position)
{
    return (p_tokens[position].type == NUM);
}

static bool is_reg(size_t position)
{
    return (p_tokens[position].type == REG);
}

static bool is_command(size_t position)
{
    return p_tokens[position].type == CMD;
}

static char is_ch(size_t position)
{
    if(p_tokens[position].type == CH)
        return p_tokens[position].value.ch;
    return 0;
}

static char is_keyword(size_t position, char name)
{
    if(p_tokens[position].type == KW)
        if(p_tokens[position].value.keyword == name)
            return name;
    return 0;
}


static bool GetG()
{
    while(!is_keyword(iterator, '$'))
    {
        if(is_lab(iterator) && is_keyword(iterator + 1, ':'))
        {
            labeled_cmd temp = {};
            temp.label       = is_lab(iterator);
            temp.to          = -1;
            temp.from        = n_cmds;

            if(!array_push(&labels, &temp))
                return false;

            iterator+=2;
        }
        else if(is_command(iterator) && is_command(iterator + 1) || 
                is_command(iterator) && is_lab(iterator + 1)     && is_keyword(iterator + 2, ':') ||
                is_command(iterator) && is_keyword(iterator + 1, '$'))
        {
           add_cmd(p_tokens[iterator].value.command, is_nothing, 0);
           iterator++;
        }
        else if(is_command(iterator)  && is_keyword(iterator + 1, '\'')
             && is_ch(iterator + 2) && is_keyword(iterator + 3, '\''))
        {
            add_cmd(p_tokens[iterator].value.command, is_char, is_ch(iterator + 2));
            iterator+=4;
        }
        else if(is_command(iterator) && is_lab(iterator + 1) && !is_keyword(iterator + 2, ':'))
        {
            labeled_cmd temp = {};
            temp.label       = is_lab(iterator + 1);
            temp.to          = n_cmds;
            temp.from        = -1;

            if(!array_push(&labels, &temp))
                return false;

            add_cmd(p_tokens[iterator].value.command, is_label, -2);
            iterator+=2;
        }
        else if(is_command(iterator) && is_reg(iterator + 1))
        {
            add_cmd(p_tokens[iterator].value.command, is_register, p_tokens[iterator + 1].value.reg);
            iterator+=2;
        }
        else if(is_command(iterator) && is_num(iterator + 1))
        {
            add_cmd(p_tokens[iterator].value.command, is_number, p_tokens[iterator + 1].value.number);
            iterator+=2;
        }
        else if(is_command(iterator) && is_keyword(iterator + 1, '['))
        {
            char command_key = p_tokens[iterator].value.command;
            iterator+=2;

            if(!GetE())
                return false;

            if(!is_keyword(iterator, ']'))
                return print_error("]");

            add_cmd(get_cmd_num("pop"), is_register,     get_reg_num("drk"));
            add_cmd(command_key,        is_ram_register, get_reg_num("drk"));
            
            iterator++;
        }
        else
        {
            return print_error("command");
        }
    }

    if(iterator > 1)
    {
        iterator-=2;
        if(is_lab(iterator) && is_keyword(iterator + 1, ':'))
            return print_error("ending program with no label");
    }

    return true;
}

static bool GetE()
{
    GetT();
    while((is_keyword(iterator, '+') || is_keyword(iterator, '-')) && !is_keyword(iterator, '$'))
    {
        if(is_keyword(iterator, '+'))
        {
            iterator++;
            if(!GetT())
                return false;

            add_cmd(get_cmd_num("add"), is_nothing, 0);
        }
        else if(is_keyword(iterator, '-'))
        {
            iterator++;
            if(!GetT())
                return false;
            
            add_cmd(get_cmd_num("sub"), is_nothing, 0);            
        }
    }
    return true;
}

static bool GetT()
{
    GetU();
    while((is_keyword(iterator, '*') || is_keyword(iterator, '/')) && !is_keyword(iterator, '$'))
    {
        if(is_keyword(iterator, '*'))
        {
            iterator++;
            if(!GetU())
                return false;

            add_cmd(get_cmd_num("mul"), is_nothing, 0);
        }
        else if(is_keyword(iterator, '/'))
        {
            iterator++;
            if(!GetU())
                return false;

            add_cmd(get_cmd_num("div"), is_nothing, 0);            
        }
    }
    return true;
}

static bool GetU()
{
    if(is_reg(iterator))
    {
        add_cmd(get_cmd_num("push"), is_register, p_tokens[iterator].value.reg);
        iterator++;
    }
    else if(is_num(iterator))
    {
        add_cmd(get_cmd_num("push"), is_number, p_tokens[iterator].value.number);
        iterator++;
    }
    else if(is_keyword(iterator, '('))
    {
        iterator++;
        if(!GetE())
            return false;

        if(!is_keyword(iterator, ')'))
            return print_error(")");

        iterator++;
    }
    else
    {
        return print_error("unary expression");
    }
    return true;
}


static int fill_labels()
{
    for(size_t i = 0; i < labels.size; i++)
    {
        labeled_cmd temp1 = *((labeled_cmd* )((char* )labels.data + i * labels.item_size));

        for(size_t j = 0; j < labels.size; j++)
        {
            labeled_cmd temp2 = *((labeled_cmd* )((char* )labels.data + j * labels.item_size));

            if(!strcmp(temp1.label, temp2.label) && i != j)
            {
                if(temp1.to == -1 && temp2.to == -1)
                {
                    fprintf(stderr, "Label %s: is used more than one time.\n", temp1.label);
                    return 0;
                }
                if(temp1.from == -1 && temp2.to == -1)
                {
                    cmds[temp1.to].argument = temp2.from;
                }
            }
        }

        if(temp1.from == -1)
        {
            if(cmds[temp1.to].argument == -2)
            {
                fprintf(stderr, "Label %s: is not found.\n", temp1.label);
                return 0;
            }
        }
    }
    return 1;
}


static int write_cmds(const char* binfile_path, const Command* p_commands, const size_t command_number) 
{
    assert(binfile_path);

    FILE* binfile = fopen(binfile_path, "wb");
    if(!binfile)
        return 0;

    fwrite(&command_number, sizeof(size_t), 1, binfile);
    fwrite(p_commands, sizeof(Command), command_number, binfile);

    fclose(binfile);
    return true;
}

bool create_binfile(const char* binfile_path, token* tokens)
{
    if(!array_ctor(&labels, sizeof(labeled_cmd)))
        return false;

    p_tokens = tokens;

    cmds = (Command* )calloc(cmds_size, sizeof(Command));
    if(!cmds)
    {
        array_dtor(&labels, 0);
        free(cmds);
        return false;
    }

    if(!GetG())
    {
        array_dtor(&labels, 0);
        free(cmds);
        return false;
    }

    if(!fill_labels())
    {
        array_dtor(&labels, 0);
        free(cmds);
        return false;
    }

    write_cmds(binfile_path, cmds, n_cmds);

    if(!commands_dump("logfiles/cmds_dump.txt"))
    {
        array_dtor(&labels, 0);
        free(cmds);
        return false;
    }

    array_dtor(&labels, 0);
    free(cmds);

    return true;
}

static int commands_dump(const char* dump_file_path) 
{
    const time_t        seconds = time(nullptr);
    const tm     *const date    = localtime(&seconds);
    FILE         *const logfile = fopen(dump_file_path, "w");
    if(!logfile)
        return 0;

    char day[16] = "";

    switch (date->tm_wday)
    {
        case 0: 
            strcpy(day, "Sunday"   );   
            break;
        case 1: 
            strcpy(day, "Monday"   );   
            break;
        case 2: 
            strcpy(day, "Tuesday"  );   
            break;
        case 3: 
            strcpy(day, "Wednesday");   
            break;
        case 4: 
            strcpy(day, "Thursday" );   
            break;
        case 5: 
            strcpy(day, "Friday"   );   
            break;
        case 6: 
            strcpy(day, "Saturday" );   
            break;
        default:
            break;
    }

    fprintf(logfile,
    "ASSEMBLER LOGFILE\n"
    "Time: %02d:%02d:%02d\n"
    "Day:  %s\n"
    "====================================================================\n"
    "|    |Type of argument:   |Command                                 |\n"
    "====================================================================\n",
    date->tm_hour, date->tm_min, date->tm_sec, day);
    
    for (size_t i = 0; i < n_cmds; i++)
    {
        fprintf(logfile, "|%-4zu|", i);
        switch(cmds[i].argument_type)
        {
            case is_register:
                fprintf(logfile, "REGISTER            |    %-6s%-30s|\n", get_cmd_string(cmds[i].key), get_reg_string((char)cmds[i].argument));
                break;
            case is_number:
                fprintf(logfile, "NUMBER              |    %-6s%-30lg|\n", get_cmd_string(cmds[i].key), cmds[i].argument);
                break;
            case is_ram_register:
                fprintf(logfile, "RAM                 |    %-5s[%-4s]                         |\n", get_cmd_string(cmds[i].key), get_reg_string((char)cmds[i].argument));
                break;
            case is_char:
                fprintf(logfile, "CHAR                |    %-6s%-30c|\n", get_cmd_string(cmds[i].key), (char)cmds[i].argument);
                break;
            case is_nothing:
                fprintf(logfile, "NOTHING             |    %-36s|\n", get_cmd_string(cmds[i].key));
                break;
            case is_label:
                fprintf(logfile, "LABEL               |    %-6s%-30d|\n", get_cmd_string(cmds[i].key), (int)cmds[i].argument);
                break;
            default:
                fprintf(stderr, "%zu|%lg|%d|%d\n", i, cmds[i].argument, cmds[i].argument_type, cmds[i].key);
                assert(0);
                break;
        }
        fprintf(logfile, "====================================================================\n");
    }

    fclose(logfile);  
    return 1;                                                 
}
