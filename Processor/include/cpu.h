#ifndef PROCESSOR_CPU_H
#define PROCESSOR_CPU_H


#include "../../Стэк/include/stack.h"
#include "global.h"


struct CPU
{
   double*  Registers;
   double*  DarkRegisters;
   double*  Ram;
   char*    VRam;
   stack    Stack;
   stack    CallStack;
};


const char cpu_logfile_path[] = "logfiles/cpu_logfile.txt";


void processor(const char *const binfile_path);


#endif //PROCESSOR_CPU_H