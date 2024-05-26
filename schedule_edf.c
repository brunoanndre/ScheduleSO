#include "schedule_edf.h"
#include "list.c"
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

struct node *head = NULL;
LARGE_INTEGER frequency;
int unitTimeCounter;

// add a task to the list 
void add(char *name, int priority, int burst, int deadline) {
    Task *task = malloc(sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    task->deadline = deadline;
    insert(&head, task);
}

void sortByDeadline(struct node *head) { // order tasks by deadline
    struct node *i, *j;
    Task *tempTask;

    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->task->deadline > j->task->deadline) {
                tempTask = i->task;
                i->task = j->task;
                j->task = tempTask;
            }
        }
    }
}

double elapsedTime(LARGE_INTEGER startTime){
   LARGE_INTEGER endTime;
   QueryPerformanceCounter(&endTime);
   double totalTime = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;

   return totalTime;
}

// invoke the scheduler
void scheduleEDF() {
    sortByDeadline(head);
    struct node *temp = head;
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);
    LARGE_INTEGER startTasks;
    QueryPerformanceCounter(&startTasks);

    while (temp != NULL) {
        printf("-----------------------------------------\n");
        int currentDeadline = temp->task->deadline;
        while (temp != NULL && temp->task->deadline == currentDeadline) {
            if (temp->task->burst == 0) { // if the task is completed, it is removed from the list and the execution time is displayed
                char *name = temp->task->name;
                struct node *next = temp->next;
                delete(&head, temp->task); // remove task from the list
                printf("It took %.9f seconds to complete the task %s. \n\n", elapsedTime(startTime), name);

                if (next != NULL) {
                    temp = next;
                } else {
                    temp = head;
                    printf("----------------------------------------\n");
                    printf("It took %.9f seconds to complete the round.\n", elapsedTime(startTasks));
                    printf("----------------------------------------\n\n");
                }
            } else {
                int limitToExecute = temp->task->deadline - temp->task->burst;
                if(limitToExecute < unitTimeCounter){
                    printf("The task %s could not be performed, therefore it has been discarted.\n\n", temp->task->name);
                    delete(&head, temp->task); 
                    temp = temp->next;
                }else{
                    runEDF(temp->task, QUANTUM);
                    unitTimeCounter += QUANTUM;
                    temp->task->burst -= QUANTUM;
                    if (temp->next != NULL && temp->next->task->deadline == currentDeadline) {
                        temp = temp->next;
                    } else {
                        temp = head;
                        printf("----------------------------------------\n");
                        printf("It took %.9f seconds to complete the round.\n", elapsedTime(startTasks));
                        printf("----------------------------------------\n\n");
                        QueryPerformanceCounter(&startTasks);
                    }
                }
            }
            printf("----------------------------------------\n");
            printf("%d units of elapsed time\n",unitTimeCounter);
            printf("----------------------------------------\n\n");
        }
        if (temp != NULL) {
            currentDeadline = temp->task->deadline;
        }
        
    }
    printf("----------------------------------------\n");
    printf("It took %.9f seconds to complete all tasks.\n", elapsedTime(startTime));
    printf("----------------------------------------\n");
}