/* CIOBANU Andrei-Bogdan - 312CB */

#include "cmd_handlers.h"
#include "book.h"

#include <stdlib.h>
#include <string.h>

#define DELIMITERS ":\n"

/* insereaza cartea in trie si seteaza informatia la finalul caii */
int books_trie_insert(struct trie_node *books_trie,
                      char *title, char *author, int rating, int pages)
{
    /* insereaza title in trie, daca nu exista deja */
    void **info_book = trie_insert(books_trie, title);
    if (info_book == NULL) {
        fprintf(stderr, "books trie insertion failed!\n");
        return -1;
    }

    /* cartea exista deja */
    if (*info_book != NULL)
        return 0;

    /* seteaza informatia la finalul caii */
    *info_book = create_book(title, author, rating, pages);
    if (*info_book == NULL) {
        fprintf(stderr, "struct book creation failed!\n");
        return -1;
    }

    return 1;
}

/* insereaza autorul in trie si adauga cartea sa in arborele de carti aferent */
int authors_trie_insert(struct trie_node *authors_trie,
                        char *title, char *author, int rating, int pages)
{
    /* insereaza autorul in trie, daca nu exista deja */
    void **info_books_trie = trie_insert(authors_trie, author);
    if (info_books_trie == NULL) {
        fprintf(stderr, "authors trie insertion failed!\n");
        return -1;
    }

    /* autorul este nou, nu a mai avut carti introduse pana acum */
    if (*info_books_trie == NULL) {
        *info_books_trie = create_trie_node();
        if (*info_books_trie == NULL) {
            fprintf(stderr, "trie node memory allocation failed!\n");
            return -1;
        }
    }

    /* insereaza cartea in arborele de carti corespunzator autorului */
    return books_trie_insert(*info_books_trie, title, author, rating, pages);
}

/* adaugare recomandare carte */
int add_book(struct trie_node **tries)
{
    char *title = strtok(NULL, DELIMITERS);
    char *author = strtok(NULL, DELIMITERS);
    int rating = atoi(strtok(NULL, DELIMITERS));
    int pages = atoi(strtok(NULL, DELIMITERS));

    /* adauga cartea in T1 */
    int return_val = books_trie_insert(tries[0], title, author, rating, pages);
    if (return_val == -1)
        return -1;

    /* adauga autorul in T2 si insereaza cartea in arborele de carti potrivit */
    return_val = authors_trie_insert(tries[1], title, author, rating, pages);
    if (return_val == -1)
        return -1;

    return 1;
}

/* cautare carte */
int search_book(struct trie_node *trie, FILE *output)
{
    char *title = strtok(NULL, DELIMITERS);

    /* auto-complete pentru cautare carte */
    if (title[strlen(title) - 1] == '~') {
        int res = trie_autocomplete(trie, output, title, MAX_BOOK_TITLE_LEN, 3);
        if (res == 0)
            fprintf(output, "Nicio carte gasita.\n");

        return res;
    }

    /* se cauta in T1 dupa titlul cartii */
    struct book *info = trie_search_info(trie, title);
    if (info == NULL)
        fprintf(output, "Cartea %s nu exista in recomandarile tale.\n", title);
    else
        fprintf(output, "Informatii recomandare: %s, %s, %d, %d\n",
                title, info->author, info->rating, info->pages);

    return 1;
}

/* afisare carti autor */
int list_author(struct trie_node *trie, FILE *output)
{
    char *author = strtok(NULL, DELIMITERS);

    /* auto-complete pentru cautare autor */
    if (author[strlen(author) - 1] == '~') {
        int res = trie_autocomplete(trie, output, author, MAX_AUTHOR_LEN, 3);
        if (res == 0)
            fprintf(output, "Niciun autor gasit.\n");

        return 1;
    }

    /* se cauta in T2 dupa numele autorului */
    struct trie_node *books_trie = trie_search_info(trie, author);
    if (books_trie == NULL) {
        fprintf(output, "Autorul %s nu face parte din recomandarile tale.\n",
                author);
        return 1;
    }

    /* sir de caractere folosit pentru a forma output-ul de la print_trie() */
    char *string = malloc(MAX_BOOK_TITLE_LEN + 1); /* + 1 pentru '\0' */
    if (string == NULL)
        return -1;

    int n = -1; /* afiseaza tot trie-ul */
    print_trie(books_trie, output, string, 0, &n);

    free(string);

    return 1;
}

/* cautare in cartile unui autor */
int search_by_author(struct trie_node *trie, FILE *output)
{
    char *author = strtok(NULL, DELIMITERS);

    /* auto-complete pentru cautare autor */
    if (author[strlen(author) - 1] == '~') {
        int res = trie_autocomplete(trie, output, author, MAX_AUTHOR_LEN, 3);
        if (res == 0)
            fprintf(output, "Niciun autor gasit.\n");

        return res;
    }

    /* se cauta in T2 dupa numele autorului */
    struct trie_node *books_trie = trie_search_info(trie, author);
    if (books_trie == NULL) {
        fprintf(output, "Autorul %s nu face parte din recomandarile tale.\n",
                author);
        return 1;
    }

    search_book(books_trie, output);

    return 1;
}

/* stergere recomandare */
void delete_book(struct trie_node **tries, FILE *output)
{
    char *title = strtok(NULL, DELIMITERS);

    /* elimina cartea din T1 */
    struct book *returned_info = trie_remove(tries, title, 0);
    if (returned_info == NULL) {
        fprintf(output, "Cartea %s nu exista in recomandarile tale.\n", title);
        return;
    }

    /* cauta trie-ul cu cartile scrise de autorul cartii de eliminat */
    struct trie_node *books = trie_search_info(tries[1], returned_info->author);
    if (books == NULL)
        return;

    free_book(returned_info);

    /* elimina cartea din trie-ul cu carti scrise de autor */
    returned_info = trie_remove(&books, title, 0);
    if (returned_info == NULL)
        return;

    /* daca nu mai exista alte carti pentru autor, sterge autorul din T2 */
    if (books == NULL)
        trie_remove(tries + 1, returned_info->author, 0);

    free_book(returned_info);
}
