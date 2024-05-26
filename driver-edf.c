#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "schedule_edf.c"


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
    
    in = fopen("edf-schedule_pri.txt", "r");
    if(in == NULL){
        printf("erro ao abrir o arquivocr\n");
    }
    
    while (fgets(task,SIZE,in) != NULL) {
        char *temp;
        temp = strtok(task, ",");
        name = strdup(temp); 
        temp = strtok(NULL, ",");
        priority = atoi(temp);
        temp = strtok(NULL, ",");
        burst = atoi(temp);
        temp = strtok(NULL, ",");
        //Only to EDF algorithm
        deadline = atoi(temp);

        // add the task to the scheduler's list of tasks
        //add(name,priority,burst);
        //to EDF only
        add(name,priority,burst,deadline);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    scheduleEDF();

    return 0;
}
