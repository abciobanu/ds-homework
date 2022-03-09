/* CIOBANU Andrei-Bogdan - 312CB */

#include "book.h"

#include <stdlib.h>
#include <string.h>

/* creeaza o carte cu informatiile date */
struct book *create_book(char *title, char *author, int rating, int pages)
{
    struct book *new_book = malloc(sizeof(struct book));
    if (new_book == NULL)
        return NULL;

    size_t title_len = strlen(title) + 1; /* + 1 pentru '\0' */
    new_book->title = malloc(title_len);
    if (new_book->title == NULL) {
        free(new_book);
        return NULL;
    }

    size_t author_len = strlen(author) + 1; /* + 1 pentru '\0' */
    new_book->author = malloc(author_len);
    if (new_book->author == NULL) {
        free(new_book->title);
        free(new_book);
        return NULL;
    }

    memcpy(new_book->title, title, title_len);
    memcpy(new_book->author, author, author_len);

    new_book->rating = rating;
    new_book->pages = pages;

    return new_book;
}

/* elibereaza memoria ocupata de o carte */
void free_book(void *book)
{
    struct book *b = book;

    free(b->title);
    free(b->author);
    free(b);
}
