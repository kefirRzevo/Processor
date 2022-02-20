#include <cstring>
#include <cmath>
#include "../include/cpu.h"
#include "../../Стэк/include/stack.h"
#include "../include/logs_cpu.h"



static void read_commands(const char *const binfile_path, Command **const pp_commands, size_t *const p_commands_number)
{
    assert(binfile_path);
    assert(pp_commands);
    assert(p_commands_number);

    FILE* binfile = fopen(binfile_path, "rb");
    assert(binfile);

    fread(p_commands_number, sizeof(size_t), 1, binfile);

    *pp_commands = (Command *)calloc(*p_commands_number, sizeof(Command));
    assert(pp_commands);

    fread(*pp_commands, sizeof(Command), *p_commands_number, binfile);

    fclose(binfile);
}


static int run_commands(FILE** logfile, CPU* const p_cpu, Command *const p_commands, const size_t commands_number)
{
    assert(p_cpu);
    assert(p_commands);
    assert(commands_number);
    assert(*logfile);
    
    const Command *p_current_command = p_commands;

    #define DEF_CMD(cmd_name, cmd_key, arguments_number, code)      \
    case CMD_##cmd_name:                                            \
    {                                                               \
        code                                                        \
        break;                                                      \
    }

    while (p_current_command < p_commands + commands_number)
    {
        //printf("cmd_type:|%d|\narg:|%lg|\n\n", p_current_command->key, p_current_command->argument);
        switch (p_current_command->key)
        {
            #include "./commands.txt"

            default:
            {
                printf("Something went wrong!!!\n");
                exit(0x440);
            }
        }
        report_logfile(logfile, p_cpu, p_current_command);
        p_current_command += 1;
    }
    #undef DEF_CMD 

    return 0;  
}


static void CpuCtor(CPU* const p_cpu) // rename
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
    Command *p_commands = nullptr;
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