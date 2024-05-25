#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "list.h"

// length of a time quantum
#define QUANTUM 10
// run the specified task for the following time slice
void run(Task *task, int slice);

#endif // CPU_H_INCLUDED
