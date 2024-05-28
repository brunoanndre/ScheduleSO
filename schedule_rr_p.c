#include "schedule_rr_p.h"
#include "list.c"
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PRIORITIES 5

int executionTime = 0;

struct node *head = NULL;
typedef struct Queue {
    struct node *head;
} Queue;

Queue priorityQueues[NUM_PRIORITIES];
LARGE_INTEGER frequency;




void add(char *name, int priority, int burst){
   Task *task = malloc(sizeof(Task));
   task->name = name;
   task->priority = priority;
   task->burst = burst;
   insert(&head,task);
}

void addToPriorityQueue(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    insert(&(priorityQueues[priority].head), task);
}

double elapsedTime(LARGE_INTEGER startTime){
   LARGE_INTEGER endTime;
   QueryPerformanceCounter(&endTime);
   double totalTime = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;

   return totalTime;
}

void executePriorityQueue(Queue *queue) {
    struct node *temp = queue->head;
    while (temp != NULL) {
        printf("task %s",temp->task->name);
        printf("-----------------------------------------\n");
        if (temp->task->burst == 0) {
            char *name = temp->task->name;
            struct node *next = temp->next;
            delete(&(queue->head), temp->task);
            printf("It took %i ut to complete the task %s. \n", executionTime, name);
            if (next != NULL) {
                temp = next;
            } else {
                break;
            }
        } else {
            run(temp->task, QUANTUM);
            temp->task->burst -= QUANTUM;
            //temp = temp->next;
            executionTime += QUANTUM;
        }
    }
}

void schedule() {
    struct node *temp = head;
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);
    double totalTime = 0;

    for (int i = 0; i <= NUM_PRIORITIES; i++) {
        priorityQueues[i].head = NULL; 
    }

    while (temp != NULL) {
        addToPriorityQueue(temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }


    for (int i = 0; i <= NUM_PRIORITIES; ++i) { 
        if (priorityQueues[i].head != NULL) { 
            printf("Executing priority queue %d\n", i);
            executePriorityQueue(&priorityQueues[i]); 
        }
    }


    printf("----------------------------------------\n");
    printf("It took %.9f seconds to complete all tasks.\n", elapsedTime(startTime));
    printf("----------------------------------------\n");
}