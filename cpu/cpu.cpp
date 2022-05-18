#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/cpu.h"

static const char* get_cmd_string(char type);
static const char* get_reg_string(char type);
static int logfile_init (FILE** logfile);
static int logfile_print(FILE** logfile, CPU* const p_cpu, const Command *const p_current_command);

static int read_commands(const char *const binfile_path, Command **const pp_commands, size_t *const p_commands_number);
static int  run_commands(FILE** logfile, CPU* const p_cpu, Command *const p_commands, const size_t commands_number);


static int read_commands(const char *const binfile_path, Command **const pp_commands, size_t *const p_commands_number)
{
    assert(binfile_path);
    assert(pp_commands);
    assert(p_commands_number);

    FILE* binfile = fopen(binfile_path, "rb");
    if(!binfile)
        return 0;

    fread(p_commands_number, sizeof(size_t), 1, binfile);

    *pp_commands = (Command *)calloc(*p_commands_number, sizeof(Command));
    if(!*pp_commands)
        return 0;

    fread(*pp_commands, sizeof(Command), *p_commands_number, binfile);

    fclose(binfile);
    return 1;
}


static int run_commands(FILE** logfile, CPU* const p_cpu, Command *const p_commands, const size_t commands_number)
{
    assert(p_cpu);
    assert(p_commands);
    assert(commands_number);
    assert(*logfile);
    
    const Command *p_current_command = p_commands;

    #define DEF_CMD(cmd_name, cmd_key, arguments_number, code)      \
    case cmd_key:                                                   \
    {                                                               \
        code                                                        \
        break;                                                      \
    }

    while (p_current_command < p_commands + commands_number)
    {
        logfile_print(logfile, p_cpu, p_current_command);
        switch (p_current_command->key)
        {
            #include "./commands.txt"

            default:
            {
                fprintf(stderr, "Something went wrong!\n");
                return 0;
            }
        }
        p_current_command++;
    }
    #undef DEF_CMD 

    return 1;  
}


static void CpuCtor(CPU* const p_cpu)
{
    assert(p_cpu);
    
    p_cpu->Registers     = (double *)calloc(REGISTERS_NUMBER,      sizeof(double));
    p_cpu->DarkRegisters = (double *)calloc(DARK_REGISTERS_NUMBER, sizeof(double));
    p_cpu->Ram           = (double *)calloc(RAM_SIZE,              sizeof(double));
    p_cpu->VRam          = (char   *)calloc(HEIGHT * WIDTH,        sizeof(char  ));

    assert(p_cpu->Registers);
    
    StackCtor(&p_cpu->Stack);
    StackCtor(&p_cpu->CallStack);
}


static void CpuDtor(CPU* const p_cpu)
{
    assert(p_cpu);
    
    free(p_cpu->Registers);
    free(p_cpu->DarkRegisters);
    free(p_cpu->Ram);
    free(p_cpu->VRam);

    StackDtor(&p_cpu->Stack);
    StackDtor(&p_cpu->CallStack);
}


void processor(const char *const binfile_path)
{
    CPU      cpu        = {};
    Command* p_commands = nullptr;
    size_t   n_commands = 0;
    FILE*       logfile = nullptr;
    
    logfile_init(&logfile);
    CpuCtor(&cpu);
    read_commands(binfile_path, &p_commands, &n_commands);
    run_commands(&logfile, &cpu, p_commands, n_commands);
    CpuDtor(&cpu);

    free(p_commands);
    fclose(logfile);
}

static int logfile_init(FILE** logfile)
{ 
    const time_t        seconds = time(nullptr);
    const tm     *const date    = localtime(&seconds);
                       *logfile = fopen("logfiles/cpu_dump.txt", "w");
    if(!*logfile)
        return 0;

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
    "|Name_cmd:   |Argument_type:  |Argument:   |Stack size: |Last 16 stack elements:                                                                          \n"  
    "|            |                |            |            |Registers:                                                                                       \n"
    "==========================================================================================================================================================\n",
    date->tm_hour, date->tm_min, date->tm_sec, day);    
    return 1;                               
}

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

static const char* get_reg_string(char type)
{
    #define DEF_REG(name, reg)  \
        else if (reg == type)   \
            return #name;                      

                    if (0) {} 
    #include "../cpu/registers.txt"
    #undef DEF_REG

    return nullptr;
}

static int logfile_print(FILE** logfile, CPU* const p_cpu, const Command *const p_current_command) 
{
    assert(p_cpu);
    assert(p_current_command);
    assert(*logfile);


    fprintf(*logfile, "|%-12s", get_cmd_string(p_current_command->key));
    switch(p_current_command->argument_type)
    {
        case is_char:
            fprintf(*logfile, "|CHAR            |%-5c(%-5lg)|", (char)p_current_command->argument, p_current_command->argument);
            break;
        case is_number:
            fprintf(*logfile, "|NUMBER          |%-12lg|", p_current_command->argument);
            break;
        case is_register:
            fprintf(*logfile, "|REGISTER        |%-5s(%-5lg)|", get_reg_string((char)p_current_command->argument), p_current_command->argument);
            break;
        case is_ram_register:
            fprintf(*logfile, "|RAM             | [%-4lg]     |", p_current_command->argument);
            break;
        case is_nothing:
            fprintf(*logfile, "|NOTHING         |            |");
            break;
        case is_label:
            fprintf(*logfile, "|LABEL           |to: %-8lg|", p_current_command->argument);
            break;
        default:
            return 0;
    }

    fprintf(*logfile, "%-12zu|", p_cpu->Stack.size);

    if(p_cpu->Stack.size < 16)
        for(size_t i = 0 ; i < p_cpu->Stack.size; i++)
            fprintf(*logfile, "%-6.3lg", p_cpu->Stack.data[i]);
    else
        for(size_t i = 0 ; i < 16; i++)
            fprintf(*logfile, "%-6.3lg", p_cpu->Stack.data[i]);

    fprintf(*logfile, "\n|            |                |            |            |");

    for(char i = 0; i < REGISTERS_NUMBER; i++)
        fprintf(*logfile, "%-6s", get_reg_string(i)); 

    fprintf(*logfile, "\n|            |                |            |            |");

    for(size_t i = 0; i < REGISTERS_NUMBER; i++)
        fprintf(*logfile, "%-6.3lg", *(p_cpu->Registers + i)); 
                                           
    fprintf(*logfile, "\n----------------------------------------------------------------------------------------------------------------------------------------------------------\n");                                             
    return 1;
}
