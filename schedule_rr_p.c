#include "schedule_rr_p.h"
#include "list.c"
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

struct node *head = NULL;
LARGE_INTEGER frequency;


// add a task to the list 
void add(char *name, int priority, int burst){
   Task *task = malloc(sizeof(Task));
   task->name = name;
   task->priority = priority;
   task->burst = burst;
   insert(&head,task);
}

double elapsedTime(LARGE_INTEGER startTime){
   LARGE_INTEGER endTime;
   QueryPerformanceCounter(&endTime);
   double totalTime = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;

   return totalTime;
}

void sortByPriority(struct node *head)
{
    if(head == NULL || head->next == NULL){
        return;
    }
    struct node *sorted = NULL;
    struct node *current = head;
    
    while(current != NULL){
        struct node *next = current->next;    
        if(sorted == NULL || sorted->task->priority > current->task->priority){
            current->next = sorted;
            sorted = current;
        }else{
            struct node *temp = sorted;
            while(temp->next != NULL && temp->next->task->priority <= current->task->priority){
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// invoke the scheduler
void schedule(){
   sortByPriority(head);
   struct node *temp = head;
   QueryPerformanceFrequency(&frequency);
   LARGE_INTEGER startTime;
   QueryPerformanceCounter(&startTime);
   LARGE_INTEGER startTasks;
   QueryPerformanceCounter(&startTasks);

   while(temp != NULL){
      printf("-----------------------------------------\n");
      int currentPriority = temp->task->priority;
      while(temp != NULL && temp->task->priority == currentPriority){
         if(temp->task->burst > 0){ //Se a task estiver concluída é removida da lista e exibe tempo de execução
            //printf("funciona por favor");
            run(temp->task,QUANTUM);
            temp->task->burst -= QUANTUM;
            if(temp->task->burst == 0){ //Se a task estiver concluída é removida da lista e exibe tempo de execução
                char *name = temp->task->name;
                delete(&head,temp->task); //Remove task da lista
                printf("It took %.9f seconds to complete the task %s. \n",elapsedTime(startTime),name);
            }
            if(temp->next != NULL && temp->next->task->priority == currentPriority){
               temp = temp->next;
            }else{
               temp = head;
               printf("----------------------------------------\n");
               printf("It took %.9f seconds to complete the round.\n",elapsedTime(startTasks));
               printf("----------------------------------------\n\n");
               QueryPerformanceCounter(&startTasks);
            }
         }
      }
   }
   printf("----------------------------------------\n");
   printf("It took %.9f seconds to complete all tasks.\n",elapsedTime(startTime));
   printf("----------------------------------------\n\n");
   
}


