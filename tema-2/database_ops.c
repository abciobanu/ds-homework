/* CIOBANU Andrei-Bogdan - 312CB */

#include "database_ops.h"

#include "season.h"
#include "series.h"
#include "get.h"

#include <stdlib.h>
#include <string.h>

/* functia de comparare pentru inserarea in categoriile 1-3 */
int insertion_fcmp(void *s_a, void *s_b)
{
    struct series *series_a = s_a;
    struct series *series_b = s_b;

    if (series_a->rating == series_b->rating)
        return strcmp(series_b->name, series_a->name);

    return (series_a->rating > series_b->rating) ? 1 : -1;
}

/*
 * adauga un serial nou in categoria data la intrare (1-3); se afiseaza un mesaj
 * de forma: "Serialul <nume> a fost adaugat la pozitia %d.\n"
 */
int add_series(FILE *output, struct node **categories)
{
    struct series *new_series = get_series_from_input();
    if (new_series == NULL)
        return -1;

    struct node **assigned_category = &categories[new_series->id - 1];
    int position = insert_before(assigned_category, new_series, insertion_fcmp);
    if (position == -1) {
        free_series(&new_series);

        fprintf(stderr, "list insertion failed!\n");
        return -1;
    }

    fprintf(output, "Serialul %s a fost adaugat la pozitia %d.\n",
            new_series->name, position);

    return 1;
}

/*
 * adauga un sezon nou pentru serialul cu numele name; se afiseaza un mesaj de
 * forma: "Serialul <nume> are un sezon nou.\n"
 */
int add_season(FILE *output,
               struct node **categories,
               struct queue *watch_later,
               struct stack *currently_watching)
{
    char *series_name = strtok(NULL, DELIMITERS);

    int time = 0;
    struct season *new_season = get_a_season_from_input(&time);
    if (new_season == NULL)
        return -1;

    struct series *to_be_found = create_series(series_name, 0, 0, 0, 0, NULL);
    if (to_be_found == NULL) {
        free_season(new_season);

        fprintf(stderr, "to_be_found series creation failed!\n");
        return -1;
    }

    /* cauta serialul in toate categoriile, watch_later si currently_watching */
    struct series *found = NULL;

    for (size_t i = 0; !found && i < N_CATEGORIES; ++i)
        found = find_data(categories[i], to_be_found, cmp_series);

    if (!found)
        found = find_item_in_queue(watch_later, to_be_found, cmp_series);

    if (!found)
        found = find_item_in_stack(currently_watching, to_be_found, cmp_series);

    free_series(to_be_found);

    /* daca inserarea in coada de sezoane esueaza */
    if (insert(found->seasons, new_season) == -1) {
        free_season(new_season);

        fprintf(stderr, "insertion failed!\n");
        return -1;
    }

    ++found->no_of_seasons;
    found->watch_time_left += time;

    fprintf(output, "Serialul %s are un sezon nou.\n", series_name);
    return 1;
}

/*
 * muta un serial din cele 4 categorii in coada watch_later; afiseaza un mesaj
 * de forma: "Serialul <nume> se afla in coada de asteptare pe pozitia %d.\n"
 */
int later(FILE *output, struct node **categories, struct queue *watch_later)
{
    char *series_name = strtok(NULL, DELIMITERS);

    struct series *to_be_found = create_series(series_name, 0, 0, 0, 0, NULL);
    if (to_be_found == NULL) {
        fprintf(stderr, "to_be_found series creation failed!\n");
        return -1;
    }

    /* cauta serialul in toate categoriile (inclusiv top10) */
    struct series *found = NULL;

    for (size_t i = 0; !found && i < N_CATEGORIES - 1; ++i)
        found = remove_node(&categories[i], to_be_found, cmp_series);

    if (!found)
        found = top_remove(&categories[N_CATEGORIES - 1], to_be_found);

    free_series(to_be_found);

    /* daca inserarea in coada watch_later esueaza */
    if (insert(watch_later, found) == -1) {
        free_series(&found);

        fprintf(stderr, "insertion failed!\n");
        return -1;
    }

    fprintf(output,
            "Serialul %s se afla in coada de asteptare pe pozitia %d.\n",
            series_name, watch_later->length);

    return 1;
}

/*
 * muta un serial in stiva currently_watching daca nu exista deja, apoi
 * vizioneaza un numar de minute dintr-un serial (informatii date la intrare);
 * se afiseaza un mesaj de forma: "Serialul <nume> a fost vizionat integral.\n"
 * in cazul in care serialul a fost vizionat integral
 */
int watch(FILE *output,
          struct node **categories,
          struct queue *watch_later,
          struct stack *currently_watching,
          struct stack *history)
{
    char *series_name = strtok(NULL, DELIMITERS);
    int time = atoi(strtok(NULL, DELIMITERS));

    struct series *to_be_found = create_series(series_name, 0, 0, 0, 0, NULL);
    if (to_be_found == NULL) {
        fprintf(stderr, "to_be_found series creation failed!\n");
        return -1;
    }

    /* elimina serialul din categoria din care face parte sau din watch_later */
    struct series *found = NULL;

    /* intai verifica daca serialul se afla deja in stiva currently_watching */
    found = remove_item_from_stack(currently_watching, to_be_found, cmp_series);

    for (int i = 0; !found && i < N_CATEGORIES - 1; ++i)
        found = remove_node(&categories[i], to_be_found, cmp_series);

    if (!found)
        found = top_remove(&categories[N_CATEGORIES - 1], to_be_found);

    if (!found)
        found = remove_item_from_queue(watch_later, to_be_found, cmp_series);

    free_series(to_be_found);

    /*
     * daca timpul ramas din serial este mai mic decat timpul de vizionare
     * dorit, inseamna ca acesta va fi vizionat integral
     */
    if (found->watch_time_left - time <= 0) {
        int push_result = push(history, found);
        if (push_result == -1) {
            fprintf(stderr, "pushing to history failed!\n");
            return -1;
        }

        fprintf(output, "Serialul %s a fost vizionat integral.\n", series_name);
    } else {
        int push_result = push(currently_watching, found);
        if (push_result == -1) {
            fprintf(stderr, "pushing to currently_watching failed!\n");
            return -1;
        }

        found->watch_time_left -= time;
    }

    return 1;
}

/*
 * afiseaza lista cu serialele din categoria data la intrare, sub forma:
 * "Categoria <X>: [(<numeA>, <rA>), (<numeB>, <rB>), ...].\n"
 */
void show(FILE *output,
          struct node **categories,
          struct queue *watch_later,
          struct stack *currently_watching,
          struct stack *history)
{
    char *category = strtok(NULL, DELIMITERS);

    fprintf(output, "Categoria %s: ", category);

    if (strcmp(category, "1") == 0)
        print_list(output, categories[0], print_series);
    else if (strcmp(category, "2") == 0)
        print_list(output, categories[1], print_series);
    else if (strcmp(category, "3") == 0)
        print_list(output, categories[2], print_series);
    else if (strcmp(category, "top10") == 0)
        print_list(output, categories[3], print_series);
    else if (strcmp(category, "later") == 0)
        print_queue(output, watch_later, print_series);
    else if (strcmp(category, "watching") == 0)
        print_stack(output, currently_watching, print_series);
    else
        print_stack(output, history, print_series);
}
