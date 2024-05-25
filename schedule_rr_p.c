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

// invoke the scheduler
void schedule(){
//   printf("SortByPriority\n");
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
         if(temp->task->burst == 0){ //Se a task estiver concluída é removida da lista e exibe tempo de execução
            char *name = temp->task->name;
            struct node *next = temp->next;
            delete(&head,temp->task); //Remove task da lista
            printf("It tooks %.9f seconds to complete the task %s. \n",elapsedTime(startTime),name);
            //printf("----------------------------------------\n");
            if(next != NULL){
               temp = next;
            }else{
               temp = head;
               printf("----------------------------------------\n");
               printf("It tooks %.9f seconds to complete the round.\n",elapsedTime(startTasks));
               printf("----------------------------------------\n");
            }
         }else{
            //printf("funciona por favor");
            run(temp->task,QUANTUM);
            temp->task->burst -= QUANTUM;
            
            if(temp->next != NULL && temp->next->task->priority == currentPriority){
               temp = temp->next;
            }else{
               temp = head;
               printf("----------------------------------------\n");
               printf("It tooks %.9f seconds to complete the round.\n",elapsedTime(startTasks));
               printf("----------------------------------------\n");
               QueryPerformanceCounter(&startTasks);
            }
         }
      }
      if(temp != NULL){
         currentPriority = temp->task->priority;
      }
   }
   printf("----------------------------------------\n");
   printf("It tooks %.9f seconds to complete all tasks.\n",elapsedTime(startTime));
   printf("----------------------------------------\n");
   
}


