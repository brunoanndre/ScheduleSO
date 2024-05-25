#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};


void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);


#endif // LIST_H_INCLUDED
