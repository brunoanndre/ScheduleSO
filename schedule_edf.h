#ifndef SCHEDULERS_H_INCLUDED
#define SCHEDULERS_H_INCLUDED

#include "CPU.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

void add(char *name, int priority, int burst, int deadline);
void schedule();

#endif