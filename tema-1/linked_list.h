/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>

/* reprezentarea unei celule de lista dublu inlantuita generica */
struct node {
    void *data;
    struct node *prev;
    struct node *next;
};

/* creeaza o celula de lista */
struct node *create_node(void *data);

/* elibereaza memoria ocupata de o lista data */
void free_list(struct node **list, void (*free_data)(void *));

/* verifica daca in lista exista informatia data */
int existing_data(struct node *list, void *data, int (*cmp_f)(void *, void *));

/*
 * cauta in lista informatia data si intoarce adresa acesteia (sau NULL daca nu
 * exista in lista)
 */
void *search_data(struct node *list, void *data, int (*cmp_f)(void *, void *));

/* insereaza in lista o celula cu informatia data */
int insert_data(struct node **list, void *data, int (*cmp_f)(void *, void *));

/* elimina din lista (prima) celula care contine informatia data */
int remove_node(struct node **list, void *data, int (*cmp_f)(void *, void *),
                void (*free_data)(void *));

/* afiseaza in stream lista data */
void print_list(struct node *list, void (*print_data)(void *, FILE *),
                FILE *stream);

#endif /* LINKED_LIST_H */