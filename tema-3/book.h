/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef BOOK_H
#define BOOK_H

#define MAX_BOOK_TITLE_LEN 50
#define MAX_AUTHOR_LEN 40

/* reprezentarea unei carti */
struct book {
    char *title;
    char *author;
    int rating;
    int pages;
};

/* creeaza o carte cu informatiile date */
struct book *create_book(char *title, char *author, int rating, int pages);

/* elibereaza memoria ocupata de o carte */
void free_book(void *book);

#endif /* BOOK_H */
