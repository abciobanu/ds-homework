/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

#include <stddef.h>

/* reprezentarea unei cozi care foloseste lista generica simplu inlantuita */
struct queue {
    size_t item_size; /* dimensiunea unui element */
    int length; /* lungimea cozii */
    struct node *front; /* adresa primei celule din coada */
    struct node *rear; /* adresa ultimei celule din coada */
};

/* initializeaza o coada cu elemente de dimensiune item_size */
struct queue *initialize_queue(size_t item_size);

/* elibereaza memoria ocupata de o coada data */
void free_queue(struct queue **queue, void (*free_data)(void *));

/* insereaza item in coada queue */
int insert(struct queue *queue, void *item);

/* extrage elementul de la inceputul cozii */
int extract(struct queue *queue, void *extracted_item,
            void (*free_data)(void *));

/* concateneaza coada sursa la coada destinatie */
void concatenate(struct queue *destination, struct queue *source);

/*
 * identifica elementul din coada care indeplineste conditia data, fara a-l
 * extrage; returneaza adresa informatiei gasite
 */
void *find_item_in_queue(struct queue *queue, void *item,
                         int (*fcmp)(void *, void *));

/*
 * extrage elementul din coada care indeplineste conditia data; returneaza
 * informatia extrasa
 */
void *remove_item_from_queue(struct queue *queue, void *item,
                             int (*fcmp)(void *, void *));

/* afiseaza elementele din coada */
void print_queue(FILE *stream, struct queue *queue,
                 void (*print_item)(FILE *, void *));

#endif /* QUEUE_H */
