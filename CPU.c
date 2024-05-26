#include <stdio.h>
#include "CPU.h"


// run this task for the specified time slice
void run(Task *task, int slice) {
    printf("Running task = [%s] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst, slice);
}

void runEDF(Task *task,int slice){
    printf("Running task = [%s] [%d] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst,task->deadline, slice);
}
