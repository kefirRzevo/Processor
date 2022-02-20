#ifndef LOGS_CPU_H
#define LOGS_CPU_H


#include "global.h"
#include "cpu.h"


void logfile_init(FILE** logfile);
void report_logfile (FILE** logfile, CPU* const p_cpu, const Command *const p_current_command);


#endif //LOGS_CPU_H