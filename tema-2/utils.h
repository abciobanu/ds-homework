/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef UTILS_H
#define UTILS_H

#include "list.h"
#include "queue.h"
#include "stack.h"
#include "season.h"
#include "series.h"

#include <stdio.h>

/* initializeaza baza de date */
int initialize_database(struct node ***categories,
                        struct queue **watch_later,
                        struct stack **currently_watching,
                        struct stack **history);

/* elibereaza memoria ocupata de baza de date */
void free_database(struct node ***categories,
                   struct queue **watch_later,
                   struct stack **currently_watching,
                   struct stack **history);

/* analizeaza operatiile date la intrare */
int parse_instructions(FILE *input, FILE *output,
                       struct node **categories,
                       struct queue *watch_later,
                       struct stack *currently_watching,
                       struct stack *history);

#endif /* UTILS_H */
