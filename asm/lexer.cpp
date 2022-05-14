#include "../include/lexer.h"

static size_t create_buf(const char* file_path, char** buf);
static int    tokens_dump(const array* const tokens, const char* dump_file_path);

static void skip_spaces();
static void find_ending(size_t* token_length);

static int make_num  (array* tokens);
static int make_cmd  (array* tokens, char cmd);
static int make_reg  (array* tokens, char reg);
static int make_key  (array* tokens, char key);
static int make_char (array* tokens, char ch);
static int make_label(array* tokens, array* idents, char* label, size_t label_size);

static bool is_cmd(char* command,   size_t cmd_size);
static bool is_reg(char* registery, size_t reg_size);
static bool is_key(char  keyword);

static const char*  get_cmd_string(char type);
static const char*  get_reg_string(char type);

static char*  iterator = nullptr;
static size_t lines    = 1;


#define $$$ fprintf(stderr, "|%d|%s|\n", __LINE__, iterator);
#define $$ fprintf(stderr, "|%d|\n", __LINE__);

#undef $$
#undef $$$


static size_t create_buf(const char* file_path, char** buf)
{
    FILE* fp = fopen(file_path, "r");
    if(!fp)
        return 0;
                                      fseek(fp,  0L, SEEK_END);
    size_t file_size        = (size_t)ftell(fp) + 1;
                                      fseek(fp,  0L, SEEK_SET);

    *buf = (char *)calloc(file_size, sizeof(char));
    if(!*buf)
        return 0;

    fread(*buf, sizeof(char), file_size, fp);
    fclose(fp);
    return file_size;
}


static void skip_spaces()
{
    while(isspace(*iterator) || *iterator == ';')
    {
        while(isspace(*iterator))
        {
            if(*iterator == '\n')
                lines++;

            iterator++;
        }

        if(*iterator == ';')
        {
            iterator ++;

            while(*iterator != '\n' && *iterator != '\0')
                iterator++;

            if(*iterator != '\0')
            {
                iterator++;
                lines++;
            }
        }
    }
}


static int make_cmd(array* tokens, char cmd)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = CMD;
    temp_token.line  = lines;
    temp_token.value.command = cmd;

    if(!array_push(tokens, &temp_token))
        return 0;
    return 1;
}


static int make_reg(array* tokens, char reg)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = REG;
    temp_token.line  = lines;
    temp_token.value.reg = reg;

    if(!array_push(tokens, &temp_token))
        return 0;
    return 1;
}


static int make_num(array* tokens)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = NUM;
    temp_token.line  = lines;
    
    int n_digits = 0;
    sscanf(iterator, "%lf%n", &temp_token.value.number, &n_digits);
    iterator += n_digits;
    
    if(!array_push(tokens, &temp_token))
        return 0;
    return 1;
}


static int make_key(array* tokens, char key)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = KW;
    temp_token.line  = lines;
    temp_token.value.keyword = key;

    if(!array_push(tokens, &temp_token))
        return 0;
    return 1;
}


static int make_char(array* tokens, char ch)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = CH;
    temp_token.line  = lines;
    temp_token.value.keyword = ch;

    if(!array_push(tokens, &temp_token))
        return 0;
    return 1;
}


static int make_label(array* tokens, array* labels, char* label, size_t label_size)
{
    assert(tokens);

    token temp_token = {};
    temp_token.type  = LAB;
    temp_token.line  = lines;

    char** labels_data = (char** )labels->data;
    for(size_t i = 0; i < labels->size; i++)
    {
        if(!strncmp(label, labels_data[i], label_size) && label_size == strlen(labels_data[i]))
        {
            temp_token.value.label = labels_data[i];
            break;
        }
    }

    if(!temp_token.value.label)
    {
        char* temp_label = (char* )calloc(label_size + 1, sizeof(char));
        if(!temp_label)
            return 0;

        memcpy(temp_label, label, label_size);
        temp_label[label_size] = '\0';
        temp_token.value.label = temp_label;

        if(!array_push(labels, &temp_label))
            return 0;
    }
    
    if(!array_push(tokens, &temp_token))
        return 0;
    
    return 1;
}


static bool is_cmd(char* command, size_t cmd_size)
{
    #define DEF_CMD(name, cmd, num_arg, code)                   \
        else if (!strncmp(iterator, #name, cmd_size))           \
        {                                                       \
            *command = cmd;                                     \
            iterator += cmd_size;                               \
            return true;                                        \
        }

                    if (0) {} 
    #include "../cpu/commands.txt"
    #undef DEF_CMD

    return false;
}


static bool is_reg(char* registery, size_t reg_size)
{
    #define DEF_REG(name, reg)                                  \
        else if (!strncmp(iterator, #name, reg_size)            \
               && strncmp(iterator, "drk", 3))                  \
        {                                                       \
            *registery = reg;                                   \
            iterator += reg_size;                               \
            return true;                                        \
        }

                    if (0) {} 
    #include "../cpu/registers.txt"
    #undef DEF_REG

    return 0;
}


static bool is_key(char keyword)
{
    switch (keyword)
    {
        case '[':
        case ']':
        case '(':
        case ')':
        case ':':
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
        default: 
            return false;
    }
} 


static const char*  get_cmd_string(char type)
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
    #define DEF_REG(name, reg)  \
        else if (reg == type)   \
        return #name;                      

                    if (0) {} 
    #include "../cpu/registers.txt"
    #undef DEF_REG

    return nullptr;
}


static void find_ending(size_t* token_length)
{
    char* begin = iterator;

    while(!isspace(*iterator) && *iterator != ';' && !is_key(*iterator) && *iterator != '\0')
        iterator++;
    
    *token_length = (size_t)(iterator - begin);
                             iterator = begin;
}


#define make_cmd(A, B)                                              \
    if(!make_cmd(A, B))                                             \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }

#define make_reg(A, B)                                              \
    if(!make_reg(A, B))                                             \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }

#define make_num(A)                                                 \
    if(!make_num(A))                                                \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }

#define make_key(A, B)                                              \
    if(!make_key(A, B))                                             \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }

#define make_char(A, B)                                             \
    if(!make_char(A, B))                                            \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }

#define make_label(A, B, C, D)                                      \
    if(!make_label(A, B, C, D))                                     \
    {                                                               \
        free(buf);                                                  \
        fprintf(stderr, "Allocation error at line %d\n", __LINE__); \
        return nullptr;                                             \
    }


token* tokenize(const char* file_path, array* idents, array* tokens)
{
    char*  buf = nullptr;
    create_buf(file_path, &buf);

    if(!buf)
        return nullptr;

    array_ctor(tokens, sizeof(token ));
    array_ctor(idents, sizeof(char *));
 
    iterator = buf;

    while(*iterator != '\0')
    {
        skip_spaces();

        if(*iterator == '\0')
            break;

        size_t token_length = 0;
        char cmd = 0;
        char reg = 0;

        if(isdigit(*iterator) || *iterator == '-' && isdigit(*(iterator + 1)))
        {
            make_num(tokens);
            continue;
        }

        if(*iterator == '\'' && *(iterator + 2) == '\'')
        {
            make_key(tokens, '\'');
            make_char(tokens, *(iterator + 1));
            make_key(tokens, '\'');
            iterator+=3;
            continue;
        }

        if(is_key(*iterator))
        {
            make_key(tokens, *iterator);
            iterator++;
            continue;
        }

        find_ending(&token_length);

        if(is_cmd(&cmd, token_length))
        {
            make_cmd(tokens, cmd);
            continue;
        }

        if(is_reg(&reg, token_length))
        {
            make_reg(tokens, reg);
            continue;
        }

        make_label(tokens, idents, iterator, token_length)

        iterator += token_length;    
    }

    make_key(tokens, '$');

    free(buf);
    tokens_dump(tokens, "logfiles/token_dump.txt");

    return (token* )tokens->data;
}


#undef make_cmd
#undef make_reg
#undef make_num
#undef make_char
#undef make_key
#undef make_label


static int tokens_dump(const array* const tokens, const char* dump_file_path)
{
    assert(tokens);
    FILE* fp = fopen(dump_file_path, "w");
    if(!fp)
        return 0;
    
    dump_init(fp, tokens);
    fprintf(fp, "|====|==========|==========|========================|\n");

    for(size_t i = 0; i < tokens->size; i++)
    {
        token temp_token = *((token *)((char* )tokens->data + i * tokens->item_size));
        switch(temp_token.type)
        {
            case CMD:
                fprintf(fp, "|%-4zu|LINE: %-4zu|COMMAND   |VALUE:   %-15s|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, get_cmd_string(temp_token.value.command));
                break;
            case REG:
                fprintf(fp, "|%-4zu|LINE: %-4zu|REGISTER  |VALUE:   %-15s|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, get_reg_string(temp_token.value.reg));
                break;
            case NUM:
                fprintf(fp, "|%-4zu|LINE: %-4zu|NUMBER    |VALUE:   %-15lg|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, temp_token.value.number);
                break;
            case KW:
                fprintf(fp, "|%-4zu|LINE: %-4zu|KEYWORD   |VALUE:   %-15c|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, temp_token.value.keyword);
                break;
            case CH:
                fprintf(fp, "|%-4zu|LINE: %-4zu|CHAR      |VALUE:   %-15c|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, temp_token.value.ch);
                break;
            case LAB:
                fprintf(fp, "|%-4zu|LINE: %-4zu|LABEL     |VALUE:   %-15s|\n"
                            "|====|==========|==========|========================|\n", i + 1, temp_token.line, temp_token.value.label);
                break;
            default:
                break;
        }
    }
    return 1;
}
