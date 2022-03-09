/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef SERIES_H
#define SERIES_H

#include "queue.h"

/* reprezentarea unui serial */
struct series {
    char *name; /* numele serialului */
    unsigned int id; /* ID-ul categoriei din care face parte serialul */
    float rating; /* rating-ul serialului */
    unsigned int no_of_seasons; /* numarul de sezoane */
    int watch_time_left; /* durata ramasa din serial de vizionat */
    struct queue *seasons; /* coada de sezoane */
};

/* creeaza un serial cu specificatiile date */
struct series *create_series(char *name, unsigned int id, float rating,
                             unsigned int no_of_seasons, int time,
                             struct queue *seasons);

/* elibereaza memoria ocupata de un serial dat */
void free_series(void *series);

/* compara doua seriale dupa nume */
int cmp_series(void *series1, void *series2);

/* afiseaza un serial */
void print_series(FILE *stream, void *series);

#endif /* SERIES_H */
