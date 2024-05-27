#include "schedule_rr_p.h"
#include "list.c"
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PRIORITIES 5

struct node *head = NULL;

typedef struct Queue {
    struct node *head;
} Queue;

Queue priorityQueues[NUM_PRIORITIES];
LARGE_INTEGER frequency;
HANDLE timeThread;

int globalTime = 0;
int realtimeSleep = 0;

DWORD WINAPI timeCounter(LPVOID lpParam) {
    while (1) {
        Sleep(1000); // 1sec
        globalTime++;
        if (realtimeSleep > 0) {
            realtimeSleep--;
        }
    }
    return 0;
}

void addToPriorityQueue(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    insert(&(priorityQueues[priority].head), task);
}

void executePriorityQueue(Queue *queue, double *totalTime) {
    struct node *temp = queue->head;
    while (temp != NULL) {
        printf("-----------------------------------------\n");
        if (temp->task->burst == 0) {
            char *name = temp->task->name;
            struct node *next = temp->next;
            delete(&(queue->head), temp->task);
            printf("It took %.9f seconds to complete the task %s. \n", *totalTime, name);
            if (next != NULL) {
                temp = next;
            } else {
                break;
            }
        } else {
            if (temp->task->priority == 1 && realtimeSleep > 0) {
                printf("Task %s in real-time sleep. Skipping...\n", temp->task->name);
                temp = temp->next;
                continue;
            }
            run(temp->task, QUANTUM);
            temp->task->burst -= QUANTUM;
            temp = temp->next;
            *totalTime += QUANTUM;
        }
    }
}

void schedule() {
    struct node *temp = head;
    QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER startTime;
    QueryPerformanceCounter(&startTime);
    double totalTime = 0;

    for (int i = 1; i <= NUM_PRIORITIES; ++i) {
        priorityQueues[i - 1].head = NULL;
    }

    while (temp != NULL) {
        addToPriorityQueue(temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }

    timeThread = CreateThread(NULL, 0, timeCounter, NULL, 0, NULL);
    if (timeThread == NULL) {
        printf("Error creating time thread.\n");
        return;
    }

    for (int i = 1; i <= NUM_PRIORITIES; ++i) {
        if (priorityQueues[i - 1].head != NULL) {
            printf("Executing priority queue %d\n", i);
            executePriorityQueue(&priorityQueues[i - 1], &totalTime);
        }
    }

    WaitForSingleObject(timeThread, INFINITE);

    CloseHandle(timeThread);

    printf("----------------------------------------\n");
    printf("It took %.9f seconds to complete all tasks.\n", totalTime);
    printf("----------------------------------------\n");
}
