/* CIOBANU Andrei-Bogdan - 312CB */

#include "utils.h"
#include "database_ops.h"

#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 512

/* initializeaza baza de date */
int initialize_database(struct node ***categories,
                        struct queue **watch_later,
                        struct stack **currently_watching,
                        struct stack **history)
{
    *categories = calloc(N_CATEGORIES, sizeof(struct node *));
    if (*categories == NULL) {
        fprintf(stderr, "categories memory allocation failed!\n");
        return -1;
    }

    *watch_later = initialize_queue(sizeof(struct series));
    if (*watch_later == NULL) {
        free(*categories);

        fprintf(stderr, "watch_later memory allocation failed!\n");
        return -1;
    }

    *currently_watching = initialize_stack(sizeof(struct series));
    if (*currently_watching == NULL) {
        free(*categories);
        free_queue(&(*watch_later), free_series);

        fprintf(stderr, "currently_watching memory allocation failed!\n");
        return -1;
    }

    *history = initialize_stack(sizeof(struct series));
    if (*history == NULL) {
        free(*categories);
        free_queue(&(*watch_later), free_series);
        free_stack(&(*currently_watching), free_series);

        fprintf(stderr, "history memory allocation failed!\n");
        return -1;
    }

    return 1;
}

/* elibereaza memoria ocupata de baza de date */
void free_database(struct node ***categories,
                   struct queue **watch_later,
                   struct stack **currently_watching,
                   struct stack **history)
{
    for (size_t i = 0; i < N_CATEGORIES; ++i)
        free_list(&(*categories)[i], free_series);

    free(*categories);
    *categories = NULL;

    free_queue(watch_later, free_series);
    *watch_later = NULL;

    free_stack(currently_watching, free_series);
    *currently_watching = NULL;

    free_stack(history, free_series);
    *history = NULL;
}

/* analizeaza operatiile date la intrare */
int parse_instructions(FILE *input, FILE *output,
                       struct node **categories,
                       struct queue *watch_later,
                       struct stack *currently_watching,
                       struct stack *history)
{
    /* buffer pentru liniile din stream-ul de intrare */
    char *line_buffer = malloc(LINE_BUFFER_SIZE);
    if (line_buffer == NULL) {
        fprintf(stderr, "Line buffer memory allocation failed!\n");
        return -1;
    }

    /* se analizeaza fiecare linie din stream */
    while (fgets(line_buffer, LINE_BUFFER_SIZE, input)) {
        /* se imparte linia in token-uri separate de DELIMITERS */
        char *word = strtok(line_buffer, DELIMITERS);
        if (word == NULL)
            break;

        int return_val = 0;

        /* primul token reprezinta tipul instructiunii */
        if (strcmp(word, "add") == 0) {
            return_val = add_series(output, categories);
        } else if (strcmp(word, "add_sez") == 0) {
            return_val = add_season(output, categories, watch_later,
                                    currently_watching);
        } else if (strcmp(word, "add_top") == 0) {
            return_val = add_top(output, &categories[3]);
        } else if (strcmp(word, "later") == 0) {
            return_val = later(output, categories, watch_later);
        } else if (strcmp(word, "watch") == 0) {
            return_val = watch(output, categories, watch_later,
                               currently_watching, history);
        } else if (strcmp(word, "show") == 0) {
            show(output, categories, watch_later, currently_watching, history);
        }

        /* s-a produs o eroare la una dintre alocarile de memorie */
        if (return_val == -1) {
            free(line_buffer);
            return -1;
        }
    }

    free(line_buffer);

    return 0;
}
