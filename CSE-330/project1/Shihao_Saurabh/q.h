#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#ifndef QUEUE_Q_H
#define QUEUE_Q_H

typedef struct Item {
    int val;
    struct Item *prev;
    struct Item *next;
} Item;

typedef struct Queue {
    Item *head;
    Item *tail;
} Queue;

void PrintAction(char *act, Item *item) {
    if (item != NULL)
        printf("Action: %s %d\n", act, item->val);
    else
        printf("Action: %s\n", act);
}

void PrintQueue(Queue *queue) {
    Item *cur = queue->head;
    printf("Queue: [");
    while (cur != NULL) {
        if (cur != queue->head)
            printf(", ");
        printf("%d", cur->val);
        if (cur == queue->tail)
            break;
        cur = cur->next;
    }
    printf("]\n");
}

Item *NewItem(int val) {
    Item *item = (Item *) malloc(sizeof(Item));
    item->val = val;
    item->prev = NULL;
    item->next = NULL;
    return item;
}

void FreeItem(Item *item) {
    free(item);
}

Queue *NewQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void AddQueue(Queue *queue, Item *item) {
    PrintAction("Add", item);
    if (queue->head == NULL) {
        queue->head = item;
        queue->tail = item;
        item->prev = item;
        item->next = item;
    } else {
        item->next = queue->head;
        item->prev = queue->tail;
        queue->head->prev = item;
        queue->tail->next = item;
        queue->tail = item;
    }
    PrintQueue(queue);
}

Item *DelQueue(Queue *queue) {
    if (queue->head == NULL) {
        perror("queue empty error!!");
        exit(EXIT_FAILURE);
    } else {
        if (queue->head == queue->tail) {
            Item *res = queue->head;
            queue->head = queue->tail = NULL;
            res->prev = res->next = NULL;
            PrintAction("Del", res);
            PrintQueue(queue);
            return res;
        } else {
            Item *res = queue->head;
            Item *next = res->next;
            Item *prev = res->prev;
            next->prev = prev;
            prev->next = next;
            queue->head = next;
            res->prev = res->next = NULL;
            PrintAction("Del", res);
            PrintQueue(queue);
            return res;
        }
    }
}

void RotateQ(Queue *queue) {
    PrintAction("Rotate", NULL);
    if (queue->head == NULL) {
        perror("queue empty error!!");
        exit(EXIT_FAILURE);
    }
    queue->head = queue->head->next;
    queue->tail = queue->tail->next;
    PrintQueue(queue);
}

#endif //QUEUE_Q_H