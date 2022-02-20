#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "../include/logs_asm.h"


static bool _is_label(const CommandLexemes* const p_command_lexem)
{
    size_t label_length = strlen(p_command_lexem->command_name);

    return ((p_command_lexem->command_name[label_length - 1] == ':') && \
    p_command_lexem->argument_string == nullptr && label_length > 1);
}


void assembler_logfile (const Command *const p_commands, const CommandLexemes *const p_command_lexemes, const size_t command_lexemes_number) 
{
    const time_t        seconds = time(nullptr);
    const tm     *const date    = localtime(&seconds);
    FILE         *const logfile = fopen(assembler_logfile_path, "w");
    assert(logfile);

    char day[16] = "";

    switch (date->tm_wday)
    {
        case 0: strcpy(day, "Sunday"   );   break;
        case 1: strcpy(day, "Monday"   );   break;
        case 2: strcpy(day, "Tuesday"  );   break;
        case 3: strcpy(day, "Wednesday");   break;
        case 4: strcpy(day, "Thursday" );   break;
        case 5: strcpy(day, "Friday"   );   break;
        case 6: strcpy(day, "Saturday" );   break;
    }

        fprintf(logfile,
        "ASSEMBLER LOGFILE\n"
        "Time: %02d:%02d:%02d\n"
        "Day:  %s\n"
        "==============================================================================================================\n"
        "|    |Name_cmd:   |Cmd_key:    |Label?      |Register?   |Number?     |Nothing?    |Ram?        |Char?       |\n"
        "==============================================================================================================\n",
        date->tm_hour, date->tm_min, date->tm_sec, day);
    
    for (size_t i = 0, j = 0; i < command_lexemes_number; i++, j++)
    {
        while(_is_label(p_command_lexemes + i))
            i++;
        
        if(p_commands[j].argument_type == is_register)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |%-12s|            |            |            |            |\n"
        "|    |            |            |            |%-12lg|            |            |            |            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_command_lexemes[i].argument_string,  p_commands[j].argument);

        else if(p_commands[i].argument_type == is_number)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |            |%-12lg|            |            |            |\n"
        "|    |            |            |            |            |            |            |            |            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_commands[j].argument);

        else if(p_commands[j].argument_type == is_label)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|%-12s|            |            |            |            |            |\n"
        "|    |            |            |%-12lg|            |            |            |            |            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_command_lexemes[i].argument_string, p_commands[j].argument);

        else if(p_commands[j].argument_type == is_nothing)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |            |            |+           |            |            |\n"
        "|    |            |            |            |            |            |            |            |            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key);

        else if(p_commands[j].argument_type == is_ram_register)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |            |            |            |%-12s|            |\n"
        "|    |            |            |            |            |            |            |%-12lg|            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_command_lexemes[i].argument_string, p_commands[j].argument);

        else if(p_commands[j].argument_type == is_ram_number)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |            |            |            |%-12s|            |\n"
        "|    |            |            |            |            |            |            |%-12lg|            |\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_command_lexemes[i].argument_string, p_commands[j].argument);

        else if(p_commands[j].argument_type == is_char)
        fprintf(logfile,
        "|%-4zu|%-12s|%-12d|            |            |            |            |            |%-12s|\n"
        "|    |            |            |            |            |            |            |            |%-12lg|\n"
        "==============================================================================================================\n",
        j + 1, p_command_lexemes[i].command_name, p_commands[j].key,
        p_command_lexemes[i].argument_string, p_commands[j].argument);
    }

    fclose(logfile); 
    assert(logfile);                                                   
}