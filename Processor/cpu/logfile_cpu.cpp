#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "../include/logs_cpu.h"


void logfile_init(FILE** logfile)
{ 
    const time_t        seconds = time(nullptr);
    const tm     *const date    = localtime(&seconds);
                       *logfile = fopen(cpu_logfile_path, "w");
    assert(*logfile);

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

    fprintf(*logfile,
    "CPU LOGFILE\n"
    "Time: %02d:%02d:%02d\n"
    "Day:  %s\n"
    "==========================================================================================================================================================\n"
    "|Name_cmd:   |Argument_type:  |Argument:   |Stack size: |Last 16 stack elements:                                                                         |\n"  
    "|            |                |            |            |Registers:                                                                                      |\n"
    "==========================================================================================================================================================\n",
    date->tm_hour, date->tm_min, date->tm_sec, day);
    

    if(!(*logfile))
    {
        printf("OPEN_LOG_FILE_ERROR KILL YOURSELF MY DICK IS BIG!\n");  
        exit(3);
    }                                      
}


static void write_register(char* const argument_string, size_t num_reg)
{
    assert(argument_string);

    switch(num_reg)
    {
        case 0:  strcpy(argument_string, "rax");   break;
        case 1:  strcpy(argument_string, "rbx");   break;
        case 2:  strcpy(argument_string, "rcx");   break;
        case 3:  strcpy(argument_string, "rdx");   break;
        case 4:  strcpy(argument_string, "rbp");   break;
        case 5:  strcpy(argument_string, "rsi");   break;
        case 6:  strcpy(argument_string, "rdi");   break;
        case 7:  strcpy(argument_string, "rsp");   break;
        case 8:  strcpy(argument_string, "8r" );   break;
        case 9:  strcpy(argument_string, "9r" );   break;
        case 10: strcpy(argument_string, "01r");   break;
        case 11: strcpy(argument_string, "11r");   break;
        case 12: strcpy(argument_string, "21r");   break;
        case 13: strcpy(argument_string, "31r");   break;
        case 14: strcpy(argument_string, "41r");   break;
        case 15: strcpy(argument_string, "51r");   break;
    }
}


static void write_argument_type(char* const argument_string, size_t num_reg)
{
    assert(argument_string);

    switch(num_reg)
    {
        case 1:  strcpy(argument_string, "Register"   );   break;
        case 2:  strcpy(argument_string, "Number"     );   break;
        case 3:  strcpy(argument_string, "Label"      );   break;
        case 4:  strcpy(argument_string, "RamRegister");   break;
        case 5:  strcpy(argument_string, "RamNumber"  );   break;
        case 6:  strcpy(argument_string, "Nothing"    );   break;
        case 7:  strcpy(argument_string, "Char"       );   break;
    }
}


void report_logfile (FILE** logfile, CPU* const p_cpu, const Command *const p_current_command) 
{
    assert(p_cpu);
    assert(p_current_command);
    assert(*logfile);

    char arg_type[16]       = "";
    char reg[16]            = "";

    #define DEF_CMD(cmd_name, cmd_key, arguments_number, code)  \
    if(p_current_command->key == cmd_key)                       \
        fprintf(*logfile, "|%-12s", #cmd_name);                 \
    
    #include "./commands.txt"

    #undef DEF_CMD



    write_argument_type(arg_type, p_current_command->argument_type);

    if(p_current_command->argument_type != is_nothing)
        fprintf(*logfile, "|%-16s|%-12lg|%-12zu|", arg_type, p_current_command->argument, p_cpu->Stack.size);
    else
        fprintf(*logfile, "|%-16s|            |%-12zu|", arg_type, p_cpu->Stack.size);

    for(size_t i = 0 ; i < p_cpu->Stack.size; i++)
        fprintf(*logfile, "%-6.3lg", p_cpu->Stack.data[i]);


    fprintf(*logfile, "\n|            |                |            |            |");

    for(size_t i = 0; i < REGISTERS_NUMBER; i++)
    {
        write_register(reg, i);
        fprintf(*logfile, "%-6s", reg); 
    }

    fprintf(*logfile, "\n|            |                |            |            |");

    for(size_t i = 0; i < REGISTERS_NUMBER; i++)
        fprintf(*logfile, "%-6.3lg", *(p_cpu->Registers + i)); 
    
    
                                           
    fprintf(*logfile, "\n----------------------------------------------------------------------------------------------------------------------------------------------------------\n");                                             
}