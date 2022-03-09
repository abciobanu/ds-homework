/* CIOBANU Andrei-Bogdan - 312CB */

#include "utils.h"
#include "cmd_handlers.h"
#include "book.h"

#include <stdlib.h>
#include <string.h>

#define NO_OF_TRIES 2 /* T1 si T2 */
#define LINE_BUFFER_SIZE 256
#define CMD_SEPARATOR " "

/* initializeaza cei doi arbori de prefixe pentru gestionarea cartilor */
int initialize_tries(struct trie_node ***tries)
{
    *tries = malloc(NO_OF_TRIES * sizeof(struct trie_node *));
    if (tries == NULL) {
        fprintf(stderr, "tries pointer memory allocation failed!\n");
        return -1;
    }

    (*tries)[0] = create_trie_node();
    if ((*tries)[0] == NULL) {
        fprintf(stderr, "t1 memory allocation failed!\n");
        return -1;
    }

    (*tries)[1] = create_trie_node();
    if ((*tries)[1] == NULL) {
        free_trie((*tries)[0], free); /* info nu este setat, free este ok */

        fprintf(stderr, "t2 memory allocation failed!\n");
        return -1;
    }

    return 1;
}

/* elibereaza memoria ocupata de trie-ul de carti scrise de un autor */
void free_author_books(void *trie)
{
    free_trie(trie, free_book);
}

/* elibereaza memoria ocupata de cei doi arbori de prefixe */
void free_tries(struct trie_node **tries)
{
    free_trie(tries[0], free_book);
    free_trie(tries[1], free_author_books);
    free(tries);
}

/* analizeaza operatiile date la intrare */
int parse_instructions(FILE *input, FILE *output, struct trie_node **tries)
{
    /* buffer pentru liniile din stream-ul de intrare */
    char *line_buffer = malloc(LINE_BUFFER_SIZE);
    if (line_buffer == NULL) {
        fprintf(stderr, "Line buffer memory allocation failed!\n");
        return -1;
    }

    /* se analizeaza fiecare linie din stream */
    while (fgets(line_buffer, LINE_BUFFER_SIZE, input)) {
        /* se imparte linia in token-uri */
        char *word = strtok(line_buffer, CMD_SEPARATOR);
        if (word == NULL)
            break;

        int return_val = 0;

        /* primul token reprezinta tipul instructiunii */
        if (strcmp(word, "add_book") == 0)
            return_val = add_book(tries);
        else if (strcmp(word, "search_book") == 0)
            return_val = search_book(tries[0], output);
        else if (strcmp(word, "list_author") == 0)
            return_val = list_author(tries[1], output);
        else if (strcmp(word, "search_by_author") == 0)
            return_val = search_by_author(tries[1], output);
        else if (strcmp(word, "delete_book") == 0)
            delete_book(tries, output);

        /* s-a produs o eroare la una dintre alocarile de memorie */
        if (return_val == -1) {
            free(line_buffer);
            return -1;
        }
    }

    free(line_buffer);

    return 0;
}
