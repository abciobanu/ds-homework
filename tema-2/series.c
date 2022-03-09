/* CIOBANU Andrei-Bogdan - 312CB */

#include "series.h"
#include "season.h"

#include <stdlib.h>
#include <string.h>

/* creeaza un serial cu specificatiile date */
struct series *create_series(char *name, unsigned int id, float rating,
                             unsigned int no_of_seasons, int time,
                             struct queue *seasons)
{
    struct series *new_series = malloc(sizeof(struct series));
    if (new_series == NULL)
        return NULL;

    size_t name_len = strlen(name) + 1; /* + 1 pentru '\0' */

    new_series->name = malloc(name_len);
    if (new_series->name == NULL) {
        free(new_series);
        return NULL;
    }

    memcpy(new_series->name, name, name_len);

    new_series->id = id;
    new_series->rating = rating;
    new_series->no_of_seasons = no_of_seasons;
    new_series->watch_time_left = time;
    new_series->seasons = seasons;

    return new_series;
}

/* elibereaza memoria ocupata de un serial dat */
void free_series(void *series)
{
    if (series == NULL)
        return;

    free(((struct series *)series)->name);
    free_queue(&((struct series *)series)->seasons, free_season);
    free(series);
}

/* compara doua seriale dupa nume */
int cmp_series(void *series_a, void *series_b)
{
    return strcmp(((struct series *)series_a)->name,
                  ((struct series *)series_b)->name);
}

/* afiseaza un serial */
void print_series(FILE *stream, void *series)
{
    fprintf(stream, "(%s, %.1f)",
            ((struct series *)series)->name,
            ((struct series *)series)->rating);
}
