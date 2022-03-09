/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef SEASON_H
#define SEASON_H

#include "queue.h"

/* reprezentarea unui sezon */
struct season {
    unsigned int no_of_episodes; /* numarul de episoade */
    struct queue *episodes; /* coada de episoade */
};

/* creeaza un sezon cu proprietatile date */
struct season *create_season(unsigned int no_of_episodes,
                             struct queue *episodes);

/* elibereaza memoria ocupata de un sezon dat */
void free_season(void *season);

#endif /* SEASON_H */
