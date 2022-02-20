#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../include/stack2.h" 


#ifndef StackCanaryProtect

#ifndef StackHashProtect
#define SUCCESS                         fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s SUCCESS                     \n", p_stack, __func__); \
return      SUCCESS

#define ERROR_STACK_IS_EMPTY            fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_IS_EMPTY        \n", p_stack, __func__); \
return      ERROR_STACK_IS_EMPTY

#define ERROR_STACK_UNITIALIZED         fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_UNITIALIZED     \n", p_stack, __func__); \
return      ERROR_STACK_UNITIALIZED

#define ERROR_STACK_REINITIALIZATION    fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_REINITIALIZATION\n", p_stack, __func__); \
return      ERROR_STACK_REINITIALIZATION

#define ERROR_UNALLOCATED_MEMORY        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_UNALLOCATED_MEMORY    \n", p_stack, __func__); \
return      ERROR_UNALLOCATED_MEMORY
#endif

#ifdef StackHashProtect 
#define SUCCESS                         fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s SUCCESS                      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, p_stack->hash); \
return      SUCCESS

#define ERROR_STACK_IS_EMPTY            fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_IS_EMPTY         HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, p_stack->hash); \
return      ERROR_STACK_IS_EMPTY

#define ERROR_STACK_UNITIALIZED         fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_UNITIALIZED      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, p_stack->hash); \
return      ERROR_STACK_UNITIALIZED

#define ERROR_STACK_REINITIALIZATION    fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_REINITIALIZATION HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, p_stack->hash); \
return      ERROR_STACK_REINITIALIZATION

#define ERROR_UNALLOCATED_MEMORY        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_UNALLOCATED_MEMORY     HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, p_stack->hash); \
return      ERROR_UNALLOCATED_MEMORY
#endif

#endif
    

#ifdef StackCanaryProtect

#ifndef StackHashProtect
#define SUCCESS                         const char* canary_result_check = "";                                                                                   \
                                        Canary_Check(p_stack, &canary_result_check);                                                                            \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s SUCCESS                      %s\n", p_stack, __func__, canary_result_check); \
                                        return      SUCCESS

#define ERROR_STACK_IS_EMPTY            const char* canary_result_check = "";                                                                                   \
                                        Canary_Check(p_stack, &canary_result_check);                                                                            \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_IS_EMPTY         %s\n", p_stack, __func__, canary_result_check); \
                                        return      ERROR_STACK_IS_EMPTY

#define ERROR_STACK_UNITIALIZED         const char* canary_result_check = "";                                                                                   \
                                        Canary_Check(p_stack, &canary_result_check);                                                                            \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_UNITIALIZED      %s\n", p_stack, __func__, canary_result_check); \
                                        return      ERROR_STACK_UNITIALIZED

#define ERROR_STACK_REINITIALIZATION    const char* canary_result_check = "";                                                                                   \
                                        Canary_Check(p_stack, &canary_result_check);                                                                            \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_REINITIALIZATION %s\n", p_stack, __func__, canary_result_check); \
                                        return      ERROR_STACK_REINITIALIZATION

#define ERROR_UNALLOCATED_MEMORY        const char* canary_result_check = "";                                                                                   \
                                        Canary_Check(p_stack, &canary_result_check);                                                                            \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_UNALLOCATED_MEMORY     %s\n", p_stack, __func__, canary_result_check); \
                                        return      ERROR_UNALLOCATED_MEMORY
#endif

#ifdef StackHashProtect
#define SUCCESS                         const char* canary_result_check = "";                                                                                                                                          \
                                        Canary_Check(p_stack, &canary_result_check);                                                                                                                                   \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s SUCCESS                      %-30s      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, canary_result_check, p_stack->hash); \
                                        return      SUCCESS

#define ERROR_STACK_IS_EMPTY            const char* canary_result_check = "";                                                                                                                                          \
                                        Canary_Check(p_stack, &canary_result_check);                                                                                                                                   \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_IS_EMPTY         %-30s      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, canary_result_check, p_stack->hash); \
                                        return      ERROR_STACK_IS_EMPTY

#define ERROR_STACK_UNITIALIZED         const char* canary_result_check = "";                                                                                                                                          \
                                        Canary_Check(p_stack, &canary_result_check);                                                                                                                                   \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_UNITIALIZED      %-30s      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, canary_result_check, p_stack->hash); \
                                        return      ERROR_STACK_UNITIALIZED

#define ERROR_STACK_REINITIALIZATION    const char* canary_result_check = "";                                                                                                                                          \
                                        Canary_Check(p_stack, &canary_result_check);                                                                                                                                   \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_STACK_REINITIALIZATION %-30s      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, canary_result_check, p_stack->hash); \
                                        return      ERROR_STACK_REINITIALIZATION

#define ERROR_UNALLOCATED_MEMORY        const char* canary_result_check = "";                                                                                                                                          \
                                        Canary_Check(p_stack, &canary_result_check);                                                                                                                                   \
                                        fprintf(LOG_FILE, "STACK ADDRESS: %8p %-9s ERROR_UNALLOCATED_MEMORY     %-30s      HASH: %-10" HASH_SPECIFICATOR "\n", p_stack, __func__, canary_result_check, p_stack->hash); \
                                        return      ERROR_UNALLOCATED_MEMORY
#endif

#endif


FILE *LOG_FILE = nullptr;
bool STATUS_OPEN_LOG_FILE  = false;

#define open_log_file                           \
    if(STATUS_OPEN_LOG_FILE == false)           \
        LOG_FILE = fopen ("log_file.txt", "w"); \
    STATUS_OPEN_LOG_FILE = true


static bool  realloc_stack(stack *const p_stack, size_t capacity);
static Check Canary_Check(const stack *const p_stack, const char** canary_result_check);

#ifdef StackHashProtect
static hash_t QHash(const void *const data, const size_t data_size);
static hash_t ResultHash(const stack *const p_stack);
#endif


Check StackCtor(stack *const p_stack)
{
    open_log_file;

    assert(p_stack);

    if(p_stack->size     != 0  ||
       p_stack->capacity != 0  ||    
       p_stack->data     != nullptr) 
    {
        ERROR_STACK_REINITIALIZATION;
    }

    #ifndef StackCanaryProtect
    item_t *data = nullptr;
    data = (item_t *) calloc(INITIAL_CAPACITY, sizeof(item_t));
    #endif

    #ifdef StackCanaryProtect
    void *data = nullptr;
    data = (void *) calloc(INITIAL_CAPACITY * sizeof(item_t) + 2 * sizeof(canary_t), 1);
    #endif

    if(data == nullptr)
    {
        ERROR_UNALLOCATED_MEMORY;
    }

    #ifdef StackCanaryProtect
    p_stack->LeftCanary      = STACK_CANARY;
    p_stack->RightCanary     = STACK_CANARY;
    canary_t DataLeftCanary  = STACK_CANARY;
    canary_t DataRightCanary = STACK_CANARY;

    *(canary_t *) data = DataLeftCanary;
    *(canary_t *) ((char *) data + INITIAL_CAPACITY * sizeof(item_t) + sizeof(canary_t)) = DataRightCanary;

    #endif

    p_stack->size      = 0;
    p_stack->capacity  = INITIAL_CAPACITY;
    p_stack->data      = data;

    #ifdef StackHashProtect
    p_stack->hash = ResultHash(p_stack);
    #endif

    SUCCESS;
} 


Check StackDtor(stack *const p_stack)
{
    open_log_file;

    assert(p_stack);
    
    if(p_stack->data == nullptr)
    {
        ERROR_STACK_UNITIALIZED;
    }


    free((void *)p_stack->data);
    p_stack->size        = 0;
    p_stack->capacity    = 0;
    p_stack->data        = nullptr;

    #ifdef StackCanaryProtect
    p_stack->LeftCanary  = 0;
    p_stack->RightCanary = 0;
    #endif

    #ifdef StackHashProtect
    p_stack->hash        = 0;
    #endif

    SUCCESS;
}


Check StackPush(stack  *const p_stack, const item_t element)
{
    open_log_file;

    assert(p_stack);

    if(p_stack->data == nullptr)
    {
        ERROR_STACK_UNITIALIZED;
    }

    if(p_stack->size == p_stack->capacity)
    {
        size_t new_capacity = p_stack->capacity * 2;
        if(realloc_stack(p_stack, new_capacity) == false)
        {
            ERROR_UNALLOCATED_MEMORY;
        }
        p_stack->capacity = new_capacity;
    }

    #ifndef StackCanaryProtect
    item_t *data = p_stack->data;
    #endif
    
    #ifdef StackCanaryProtect
    item_t *data =(item_t*) ((char*) p_stack->data + sizeof(canary_t));
    #endif

    data[p_stack->size] = element;
    p_stack->size++;

    #ifdef StackHashProtect
    p_stack->hash = ResultHash(p_stack);
    #endif

    SUCCESS;
}


Check StackPop(stack *const p_stack, item_t *const p_element)
{
    open_log_file;

    assert(p_stack);
    assert(p_element);

    if(p_stack->data == nullptr)
    {
        ERROR_STACK_UNITIALIZED;
    }

    if(p_stack->size == 0)
    {
        ERROR_STACK_IS_EMPTY;
    }

    #ifndef StackCanaryProtect
    item_t *data = p_stack->data;
    #endif

    #ifdef StackCanaryProtect
    item_t *data =(item_t*) ((char*) p_stack->data + sizeof(canary_t));
    #endif

    *p_element = data[p_stack->size - 1];
    p_stack->size--;

    if(p_stack->capacity == p_stack->size * 2)
    {
        size_t new_capacity = p_stack->capacity / 2;
        if(realloc_stack(p_stack, new_capacity) == false)
        {
            ERROR_UNALLOCATED_MEMORY;
        }
        p_stack->capacity = new_capacity;
    }
    
    #ifdef StackHashProtect
    p_stack->hash = ResultHash(p_stack);
    #endif

    SUCCESS;
}


Check StackDump(const stack *const p_stack)
{
    open_log_file;

    assert(p_stack);

    if(p_stack->data == nullptr)
    {
        ERROR_STACK_UNITIALIZED;
    }

    const char OK[]  = "OK";
    const char BAD[] = "BAD";

    bool check_size     = p_stack->size     <= p_stack->capacity  && p_stack->size >= 0;
    bool check_capacity = p_stack->capacity >= p_stack->size      && p_stack->capacity > 0;
    bool check_adress   = p_stack       != nullptr;
    bool check_data     = p_stack->data != nullptr;

    #ifdef StackHashProtect
    bool check_hash     = p_stack->hash == ResultHash(p_stack);
    #endif

    #ifdef StackCanaryProtect
    item_t *data = (item_t *) (p_stack->data);
    canary_t DataLeftCanary  = *(canary_t *) data;
    canary_t DataRightCanary = *(canary_t *) ((char *) data + p_stack->capacity * sizeof(item_t) + sizeof(canary_t));
    data = (item_t*) ((char*) p_stack->data + sizeof(canary_t));
    bool check_left_canary       = p_stack->LeftCanary      == STACK_CANARY;
    bool check_right_canary      = p_stack->RightCanary     == STACK_CANARY;
    bool check_data_left_canary  = DataLeftCanary  == STACK_CANARY;
    bool check_data_right_canary = DataRightCanary == STACK_CANARY;
    #endif

    #ifndef StackCanaryProtect
    item_t *data = p_stack->data;
    #endif
    
    fprintf (LOG_FILE, "\n| Stack information:                  |\n"
               "| address:           %-14p%3s|\n"
               "| size:              %-14zu%3s|\n"
               "| capacity:          %-14zu%3s|\n"
               "| data:              %-14p%3s|\n"

        #ifdef StackHashProtect
               "| hash:              %-14" HASH_SPECIFICATOR "%3s|\n"
        #endif

        #ifdef StackCanaryProtect
               "| left_canary:       %-14" CANARY_SPECIFICATOR "%3s|\n"
               "| right_canary:      %-14" CANARY_SPECIFICATOR "%3s|\n"
               "| data_left_canary:  %-14" CANARY_SPECIFICATOR "%3s|\n"
               "| data_right_canary: %-14" CANARY_SPECIFICATOR "%3s|\n"
        #endif
,
        p_stack,                  check_adress            ? OK : BAD,
        p_stack->size,            check_size              ? OK : BAD,
        p_stack->capacity,        check_capacity          ? OK : BAD,
        p_stack->data,            check_data              ? OK : BAD

        #ifdef StackHashProtect
,       p_stack->hash,            check_hash              ? OK : BAD
        #endif

        #ifdef StackCanaryProtect

,       p_stack->LeftCanary,      check_left_canary       ? OK : BAD,    
        p_stack->RightCanary,     check_right_canary      ? OK : BAD,
                 DataLeftCanary,  check_data_left_canary  ? OK : BAD,
                 DataRightCanary, check_data_right_canary ? OK : BAD
        #endif
);


    for(size_t i = 1; i <= p_stack->size; i++)
        fprintf(LOG_FILE, "| *%zu:\t%10" SPECIFICATOR " |\n", i, data[i - 1]);

    for(size_t i = p_stack->size + 1; i <= p_stack->capacity; i++)
        fprintf(LOG_FILE, "|  %zu:\t%10" SPECIFICATOR " |\n", i, data[i - 1]);
    
    fprintf(LOG_FILE, "\n");

    SUCCESS;
}


static bool realloc_stack(stack *const p_stack, size_t new_capacity)
{
    assert(p_stack);

    const item_t *const data = (item_t *)p_stack->data;

    #ifndef StackCanaryProtect
    item_t *p_new_elements = (item_t *) realloc ((void *)data, new_capacity * sizeof(item_t));
    #endif

    #ifdef StackCanaryProtect
    //*(canary_t *) ((char *)p_stack->data + p_stack->capacity * sizeof(item_t) + sizeof(canary_t)) = 0;
    item_t *p_new_elements = (item_t *) realloc ((void *)data, new_capacity * sizeof(item_t) + 2 * sizeof(canary_t));
    #endif

    if(p_new_elements == nullptr)
        return false;

    #ifndef StackCanaryProtect
    p_stack->data = p_new_elements;
    #endif

    #ifdef StackCanaryProtect
    *(canary_t *) ((char *)p_new_elements + new_capacity * sizeof(item_t) + sizeof(canary_t)) = STACK_CANARY;
    *(canary_t *) p_new_elements = STACK_CANARY;
    p_stack->data = (void *)p_new_elements;
    #endif

    return true;
}


#ifdef StackCanaryProtect
static Check Canary_Check(const stack *const p_stack, const char** canary_result_check)
{
    assert(p_stack);

    if(p_stack->data == nullptr)
    {
        *canary_result_check = "STACK_CANARY_STACK_UNITIALIZED";
        return STACK_CANARY_STACK_UNITIALIZED;
    }

    item_t *data = (item_t *)p_stack->data;
    canary_t DataLeftCanary  = *(canary_t *) data;
    canary_t DataRightCanary = *(canary_t *) ((char *) data + p_stack->capacity * sizeof(item_t) + sizeof(canary_t));
                    
    if(p_stack->LeftCanary      != STACK_CANARY || 
       p_stack->RightCanary     != STACK_CANARY ||
                DataLeftCanary  != STACK_CANARY ||
                DataRightCanary != STACK_CANARY)
    {
        *canary_result_check = "STACK_CANARY_BAD";
        return STACK_CANARY_BAD;
    }
    *canary_result_check = "STACK_CANARY_OK";

    return STACK_CANARY_OK;  
}
#endif


#ifdef StackHashProtect
static hash_t QHash(const void *const data, const size_t data_size)
{
    const char       *byte     = (char *) data;
    const char *const lastByte = byte + data_size;
    hash_t            hash     = 0;
    
    while (byte < lastByte)
        hash = ((hash << 0x8) + (hash >> 0x8)) ^ *byte++;
    
    return hash;
}


static hash_t ResultHash(const stack *const p_stack)
{
    hash_t data_hash  = 0, 
           stack_hash = 0;

    #ifndef StackCanaryProtect
    stack_hash = QHash(p_stack, 2 * sizeof(size_t) + sizeof(item_t*));
    #else
    stack_hash = QHash((item_t*)((char *)p_stack + sizeof(canary_t)), 2 * sizeof(int) + sizeof(item_t*));
    #endif

    #ifndef StackCanaryProtect
    data_hash = QHash(p_stack->data, p_stack->capacity * sizeof(item_t));
    #else
    data_hash = QHash((item_t*)((char *)p_stack->data + sizeof(canary_t)), p_stack->capacity * sizeof(item_t));
    #endif

    return abs(data_hash - stack_hash);
}
#endif