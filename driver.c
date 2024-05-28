#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "schedule_rr_p.c"

//#include "schedule_edf.h"

#define SIZE 100

int main(int argc, char *argv[]){
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int deadline;

    //in = fopen(argv[1],"r");
    in = fopen("rr-schedule_pri.txt", "r");
    if(in == NULL){
        printf("erro ao abrir o arquivocr\n");
    }
    while (fgets(task,SIZE,in) != NULL) {
        char *temp;
        temp = strtok(task, ",");
        name = strdup(temp); // Faz uma c�pia da string usando strdup
        temp = strtok(NULL, ",");
        priority = atoi(temp);
        temp = strtok(NULL, ",");
        burst = atoi(temp);
        
        // add the task to the scheduler's list of tasks
        add(name,priority,burst);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}
