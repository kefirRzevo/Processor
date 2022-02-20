#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <assert.h>
#include "config.h"


#define SPECIFICATOR "lg"
#define CANARY_SPECIFICATOR "llX"
#define HASH_SPECIFICATOR "zu"

const size_t INITIAL_CAPACITY = 1;
#ifdef StackCanaryProtect
const canary_t STACK_CANARY = 0xAB9876543210;
#endif

typedef double item_t;


#ifdef StackCanaryProtect
typedef long long int canary_t;
#endif

#ifdef StackHashProtect
typedef size_t hash_t;
#endif


struct stack
{
    #ifdef StackCanaryProtect
    canary_t LeftCanary;
    void* data;
    #endif

    #ifndef StackCanaryProtect
    item_t* data;
    #endif

    size_t size;
    size_t capacity;

    #ifdef StackHashProtect
    hash_t hash;
    #endif

    #ifdef StackCanaryProtect
    canary_t RightCanary;
    #endif
};


enum Check
{
    SUCCESS                         = 0,
    ERROR_STACK_UNITIALIZED         = 1,
    ERROR_STACK_REINITIALIZATION    = 2,
    ERROR_STACK_IS_EMPTY            = 3,
    ERROR_UNALLOCATED_MEMORY        = 4,
    STACK_CANARY_BAD                = 5,
    STACK_CANARY_OK                 = 6,
    STACK_CANARY_STACK_UNITIALIZED  = 7,
    OPEN_LOG_FILE_ERROR             = 8
};


Check StackCtor(stack *const p_stack);

Check StackDtor(stack *const p_stack);

Check StackPush(stack *const p_stack, const item_t element);
 
Check StackPop(stack *const p_stack, item_t *const p_element);

Check StackDump(const stack *const p_stack);


#endif //STACK_H