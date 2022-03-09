/* CIOBANU Andrei-Bogdan - 312CB */

#include "get.h"

#include <stdlib.h>
#include <string.h>

#define DELIMITERS " \n"

/*
 * returneaza o coada de episoade formata din datele de intrare; adauga durata
 * totala a episoadelor la time
 */
struct queue *get_episodes_from_input(unsigned int no_of_episodes, int *time)
{
    struct queue *episodes = initialize_queue(sizeof(unsigned int *));
    if (episodes == NULL) {
        fprintf(stderr, "episodes queue initialization failed!\n");
        return NULL;
    }

    for (unsigned int i = 0; i < no_of_episodes; ++i) {
        int *new_episode = malloc(sizeof(int));
        if (new_episode == NULL) {
            free_queue(&episodes, free);

            fprintf(stderr, "new_episode memory allocation failed!\n");
            return NULL;
        }

        *new_episode = atoi(strtok(NULL, DELIMITERS));

        /* daca inserarea in coada de episoade esueaza */
        if (insert(episodes, new_episode) == -1) {
            free_queue(&episodes, free);

            fprintf(stderr, "insertion failed!\n");
            return NULL;
        }

        *time += *new_episode;
    }

    return episodes;
}

/*
 * returneaza un sezon format din datele de intrare; time este folosit pentru a
 * calcula durata totala a sezonului (a episoadelor din sezon)
 */
struct season *get_a_season_from_input(int *time)
{
    unsigned int no_of_episodes = atoi(strtok(NULL, DELIMITERS));

    struct queue *episodes = get_episodes_from_input(no_of_episodes, time);
    if (episodes == NULL)
        return NULL;

    struct season *new_season = create_season(no_of_episodes, episodes);
    if (new_season == NULL) {
        free_queue(&episodes, free);

        fprintf(stderr, "new_season memory allocation failed!\n");
        return NULL;
    }

    return new_season;
}

/*
 * returneaza o coada de sezoane formate din datele de intrare; time este
 * folosit pentru a calcula durata totala a sezoanelor
 */
struct queue *get_seasons_from_input(unsigned int no_of_seasons, int *time)
{
    struct queue *seasons = initialize_queue(sizeof(struct season));
    if (seasons == NULL) {
        fprintf(stderr, "seasons memory allocation failed!\n");
        return NULL;
    }

    for (unsigned int i = 0; i < no_of_seasons; ++i) {
        struct season *new_season = get_a_season_from_input(time);
        if (new_season == NULL) {
            free_queue(&seasons, free_season);
            return NULL;
        }

        /* daca inserarea in coada de sezoane esueaza */
        if (insert(seasons, new_season) == -1) {
            free_queue(&seasons, free_season);

            fprintf(stderr, "insertion failed!\n");
            return NULL;
        }
    }

    return seasons;
}

/* returneaza un serial format din datele de intrare */
struct series *get_series_from_input()
{
    unsigned int id = atoi(strtok(NULL, DELIMITERS));
    char *name = strtok(NULL, DELIMITERS);
    float rating = atof(strtok(NULL, DELIMITERS));
    unsigned int no_of_seasons = atoi(strtok(NULL, DELIMITERS));
    int time = 0;

    struct queue *seasons = get_seasons_from_input(no_of_seasons, &time);
    if (seasons == NULL)
        return NULL;

    struct series *new_series = create_series(name, id, rating, no_of_seasons,
                                              time, seasons);
    if (new_series == NULL) {
        free_queue(&seasons, free_season);

        fprintf(stderr, "new_series creation failed!\n");
        return NULL;
    }

    return new_series;
}
