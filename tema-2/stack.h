/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef STACK_H
#define STACK_H

#include "list.h"

#include <stddef.h>

/* reprezentarea unei stive care foloseste lista generica simplu inlantuita */
struct stack {
    size_t item_size; /* dimensiunea unui element */
    int height; /* inaltimea stivei */
    struct node *top; /* adresa celulei din varf */
};

/* initializeaza o stiva cu elemente de dimensiune item_size */
struct stack *initialize_stack(size_t item_size);

/* elibereaza memoria ocupata de o stiva data */
void free_stack(struct stack **stack, void (*free_data)(void *));

/* insereaza item in stiva stack */
int push(struct stack *stack, void *item);

/* extrage elementul din varful stivei */
int pop(struct stack *stack, void *extracted_item, void (*free_data)(void *));

/* rastoarna stiva sursa in stiva destinatie */
void flip(struct stack *destination, struct stack *source);

/*
 * identifica elementul din stiva care indeplineste conditia data, fara a-l
 * extrage; returneaza adresa informatiei gasite
 */
void *find_item_in_stack(struct stack *stack, void *item,
                         int (*fcmp)(void *, void *));

/*
 * extrage elementul din stiva care indeplineste conditia data; returneaza
 * informatia extrasa
 */
void *remove_item_from_stack(struct stack *stack, void *item,
                             int (*fcmp)(void *, void *));

/* afiseaza elementele din stiva */
void print_stack(FILE *stream, struct stack *stack,
                 void (*print_item)(FILE *, void *));

#endif /* STACK_H */
