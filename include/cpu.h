#ifndef PROCESSOR_CPU_H
#define PROCESSOR_CPU_H


#include "res/stack.h"
#include "parser.h"

const size_t DARK_REGISTERS_NUMBER  = 4;
const size_t REGISTERS_NUMBER       = 17;
const size_t RAM_SIZE               = 128;  
const size_t WIDTH                  = 100;
const size_t HEIGHT                 = 60;

struct CPU
{
   double*  Registers;
   double*  DarkRegisters;
   double*  Ram;
   char*    VRam;
   stack    Stack;
   stack    CallStack;
};

void processor(const char *const binfile_path);


#endif //PROCESSOR_CPU_H