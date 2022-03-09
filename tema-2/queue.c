/* CIOBANU Andrei-Bogdan - 312CB */

#include "queue.h"

#include <stdlib.h>
#include <string.h>

/* initializeaza o coada cu elemente de dimensiune item_size */
struct queue *initialize_queue(size_t item_size)
{
    struct queue *new_queue = malloc(sizeof(struct queue));
    if (new_queue == NULL)
        return NULL;

    new_queue->item_size = item_size;
    new_queue->length = 0;
    new_queue->front = NULL;
    new_queue->rear = NULL;

    return new_queue;
}

/* elibereaza memoria ocupata de o coada data */
void free_queue(struct queue **queue, void (*free_data)(void *))
{
    if (*queue == NULL)
        return;

    free_list(&(*queue)->front, free_data);
    free(*queue);

    *queue = NULL;
}

/* insereaza item in coada queue */
int insert(struct queue *queue, void *item)
{
    struct node *new_node = create_node(item);
    if (new_node == NULL)
        return -1;

    new_node->next = NULL;

    if (queue->front == NULL)
        queue->front = new_node;
    else
        queue->rear->next = new_node;

    queue->rear = new_node;

    ++queue->length;

    return 1;
}

/* extrage elementul de la inceputul cozii */
int extract(struct queue *queue, void *extracted_item,
            void (*free_data)(void *))
{
    if (queue->front == NULL)
        return 0;

    struct node *old_front = queue->front;
    queue->front = old_front->next;

    memcpy(extracted_item, old_front->data, sizeof(queue->item_size));

    free_data(old_front->data);
    free(old_front);

    /* coada a avut un singur element */
    if (queue->front == NULL)
        queue->rear = NULL;

    --queue->length;

    return 1;
}

/* concateneaza coada sursa la coada destinatie */
void concatenate(struct queue *destination, struct queue *source)
{
    if (source->front == NULL)
        return;

    if (destination->front == NULL) {
        destination->front = source->front;
        destination->rear = source->rear;
    } else {
        destination->rear->next = source->front;
        destination->rear = source->rear;
    }

    source->front = NULL;
    source->rear = NULL;
}

/*
 * identifica elementul din coada care indeplineste conditia data, fara a-l
 * extrage; returneaza adresa informatiei gasite
 */
void *find_item_in_queue(struct queue *queue, void *item,
                         int (*fcmp)(void *, void *))
{
    if (queue->front == NULL)
        return NULL;

    void *found_item = NULL;

    struct queue *temp = initialize_queue(queue->item_size);
    if (temp == NULL)
        return NULL;

    /* muta elementele din queue in temp */
    while (queue->front != NULL) {
        if (fcmp(queue->front->data, item) == 0)
            found_item = queue->front->data;

        struct node *new_rear = queue->front;
        queue->front = queue->front->next;

        if (temp->front == NULL)
            temp->front = new_rear;
        else
            temp->rear->next = new_rear;

        temp->rear = new_rear;

        new_rear->next = NULL;
    }

    /* reformeaza queue */
    concatenate(queue, temp);

    free(temp);

    return found_item;
}

/*
 * extrage elementul din coada care indeplineste conditia data; returneaza
 * informatia extrasa
 */
void *remove_item_from_queue(struct queue *queue, void *item,
                             int (*fcmp)(void *, void *))
{
    if (queue->front == NULL)
        return NULL;

    void *removed_item = NULL;

    struct queue *temp = initialize_queue(queue->item_size);
    if (temp == NULL)
        return NULL;

    /* muta elementele din queue in temp */
    while (queue->front != NULL) {
        /*
         * daca s-a gasit elementul care indeplineste conditia data, nu il muta
         * in temp; extrage informatia si elibereaza memoria ocupata de celula
         */
        if (fcmp(item, queue->front->data) == 0) {
            struct node *removed_node = queue->front;
            queue->front = removed_node->next;

            removed_item = removed_node->data;
            free(removed_node);

            --queue->length;
        } else {
            struct node *new_rear = queue->front;
            queue->front = queue->front->next;

            if (temp->front == NULL)
                temp->front = new_rear;
            else
                temp->rear->next = new_rear;

            temp->rear = new_rear;

            new_rear->next = NULL;
        }
    }

    /* reformeaza queue */
    concatenate(queue, temp);

    free(temp);

    return removed_item;
}

/* afiseaza elementele din coada */
void print_queue(FILE *stream, struct queue *queue,
                 void (*print_item)(FILE *, void *))
{
    print_list(stream, queue->front, print_item);
}
