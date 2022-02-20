#include "../include/assembler.h"
#include "../include/global.h"
#include "../include/lexer.h"
#include "../include/parser.h"  
#include "../include/logs_asm.h"   


static size_t get_file_size(FILE* const file)
{
    assert(file);

    size_t file_size = 0;
    size_t current_position = 0;

    current_position = (size_t) ftell(file);
    fseek(file, 0L, SEEK_END);
    file_size = (size_t) ftell(file);
    fseek(file, (long) current_position, SEEK_SET);

    return file_size;
}


static char* get_asmcode(const char asmfile_path[], size_t *const p_asmcode_size)
{
    assert(asmfile_path);
    assert(p_asmcode_size);
    
    FILE *asmfile  = nullptr;
    char *asmcode  = nullptr;
    
    asmfile        = fopen(asmfile_path, "r");
    assert(asmfile);

    *p_asmcode_size = get_file_size(asmfile);
    asmcode         = (char *)calloc(*p_asmcode_size + 3, sizeof(char));
    assert(asmcode);
    
    fread(asmcode, sizeof(char), *p_asmcode_size, asmfile);
    asmcode[*p_asmcode_size]     = '\n';
    asmcode[*p_asmcode_size + 1] = '\0';

    *p_asmcode_size += 2;

    fclose(asmfile);
    assert(asmfile);
    
    return asmcode;
}


static void write_cmds_to_binfile(const char *const binfile_path, const Command *const p_commands, const size_t *const command_number) // TODO change name
{
    assert(binfile_path);

    FILE* const binfile = fopen(binfile_path, "wb");
    assert(binfile); 

    fwrite(command_number, sizeof(size_t), 1, binfile);
    fwrite(p_commands, sizeof(Command), *command_number, binfile);

    fclose(binfile);
}

void assembler(const char *const asmfile_path, const char *const binfile_path)
{
    assert(asmfile_path);
    assert(binfile_path);

    CommandLexemes *p_command_lexemes        = nullptr;
    Command        *p_commands               = nullptr;
    size_t          command_lexemes_number   = 0;
    size_t          command_number           = 0;

    size_t asmcode_size     = 0;
    char*  p_asmcode        = get_asmcode(asmfile_path, &asmcode_size);

    command_lexemes_number  = lex(p_asmcode, &asmcode_size, &p_command_lexemes);
    command_number          = parse(p_command_lexemes, command_lexemes_number, &p_commands);

    write_cmds_to_binfile(binfile_path, p_commands, &command_number);

    assembler_logfile(p_commands, p_command_lexemes, command_lexemes_number); 
    free(p_command_lexemes);
    free(p_commands);
    free(p_asmcode);
}