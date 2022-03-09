/* CIOBANU Andrei-Bogdan - 312CB */

#include "season.h"

#include <stdlib.h>

/* creeaza un sezon cu proprietatile date */
struct season *create_season(unsigned int no_of_episodes,
                             struct queue *episodes)
{
    struct season *new_season = malloc(sizeof(struct season));
    if (new_season == NULL)
        return NULL;

    new_season->no_of_episodes = no_of_episodes;
    new_season->episodes = episodes;

    return new_season;
}

/* elibereaza memoria ocupata de un sezon dat */
void free_season(void *season)
{
    if (season == NULL)
        return;

    free_queue(&((struct season *)season)->episodes, free);
    free(season);
}
