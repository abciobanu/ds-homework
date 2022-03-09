/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef LIST_H
#define LIST_H

#include <stdio.h>

/* reprezentarea unei celule din lista generica simplu inlantuita */
struct node {
    void *data; /* informatia din celula */
    struct node *next; /* adresa celulei urmatoare */
};

/* creeaza o celula de lista */
struct node *create_node(void *data);

/* elibereaza memoria ocupata de o lista data */
void free_list(struct node **list, void (*free_data)(void *));

/*
 * insereaza informatia data inainte de celula care respecta conditia data;
 * returneaza pozitia la care s-a facut inserarea in lista / -1 in caz de eroare
 */
int insert_before(struct node **list, void *data, int (*fcmp)(void *, void *));

/* identifica un element din lista; returneaza adresa informatiei gasite */
void *find_data(struct node *list, void *data, int (*fcmp)(void *, void *));

/* scoate un element din lista; returneaza informatia din celula extrasa */
void *remove_node(struct node **list, void *data, int (*fcmp)(void *, void *));

/* afiseaza elementele unei liste date */
void print_list(FILE *stream, struct node *list,
                void (*print_data)(FILE *, void *));

#endif /* LIST_H */
