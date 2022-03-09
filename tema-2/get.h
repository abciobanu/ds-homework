/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef GET_H
#define GET_H

#include "queue.h"
#include "season.h"
#include "series.h"

/*
 * returneaza o coada de episoade formata din datele de intrare; adauga durata
 * totala a episoadelor la time
 */
struct queue *get_episodes_from_input(unsigned int no_of_episodes, int *time);

/*
 * returneaza un sezon format din datele de intrare; time este folosit pentru a
 * calcula durata totala a sezonului (a episoadelor din sezon)
 */
struct season *get_a_season_from_input(int *time);

/*
 * returneaza o coada de sezoane formate din datele de intrare; time este
 * folosit pentru a calcula durata totala a sezoanelor
 */
struct queue *get_seasons_from_input(unsigned int no_of_seasons, int *time);

/* returneaza un serial format din datele de intrare */
struct series *get_series_from_input();

#endif /* GET_H */
