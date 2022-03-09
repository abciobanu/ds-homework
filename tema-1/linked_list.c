/* CIOBANU Andrei-Bogdan - 312CB */

#include "linked_list.h"

#include <stdlib.h>

/* creeaza o celula de lista */
struct node *create_node(void *data)
{
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
        return NULL;

    new_node->next = new_node;
    new_node->prev = new_node;

    new_node->data = data;

    return new_node;
}

/* elibereaza memoria ocupata de o lista data */
void free_list(struct node **list, void (*free_data)(void *))
{
    if (*list == NULL)
        return;

    struct node *p = *list;

    do {
        struct node *aux = p;
        p = p->next;

        free_data(aux->data);
        free(aux);
    } while (p != *list);

    *list = NULL;
}

/* verifica daca in lista exista informatia data */
int existing_data(struct node *list, void *data, int (*cmp_f)(void *, void *))
{
    if (list == NULL)
        return 0;

    struct node *p = list;

    do {
        /* se identifica celula cu informatia data */
        if (cmp_f(p->data, data) == 0)
            return 1;

        p = p->next;
    } while (p != list);

    return 0;
}

/*
 * cauta in lista informatia data si intoarce adresa acesteia (sau NULL daca nu
 * exista in lista)
 */
void *search_data(struct node *list, void *data, int (*cmp_f)(void *, void *))
{
    if (!existing_data(list, data, cmp_f))
        return NULL;

    if (list == NULL)
        return NULL;

    struct node *p = list;

    do {
        /* se identifica celula cu informatia data */
        if (cmp_f(p->data, data) == 0)
            return p->data;

        p = p->next;
    } while (p != list);

    return NULL;
}

/* insereaza in lista o celula cu informatia data */
int insert_data(struct node **list, void *data, int (*cmp_f)(void *, void *))
{
    if (existing_data(*list, data, cmp_f))
        return 0;

    struct node *new_node = create_node(data);
    if (new_node == NULL)
        return -1;

    if (*list == NULL) {
        *list = new_node;
        return 1;
    }

    struct node *p = *list;

    do {
        /* se identifica celula inaintea careia trebuie facuta inserarea */
        if (cmp_f(p->data, data) > 0) {
            /* se formeaza legaturile */
            new_node->prev = p->prev;
            new_node->next = p;

            p->prev->next = new_node;
            p->prev = new_node;

            /* inserare la inceput de lista */
            if (p == *list)
                *list = new_node;

            return 1;
        }

        p = p->next;
    } while (p != *list);

    /* inserare la final de lista */
    new_node->prev = p->prev;
    new_node->next = p;

    p->prev->next = new_node;
    p->prev = new_node;

    return 1;
}

/* elimina din lista prima celula care contine informatia data */
int remove_node(struct node **list, void *data, int (*cmp_f)(void *, void *),
                void (*free_data)(void *))
{
    if (!existing_data(*list, data, cmp_f))
        return 0;

    if (*list == NULL)
        return 0;

    struct node *p = *list;

    do {
        /* se identifica celula cu informatia data */
        if (cmp_f(p->data, data) == 0) {
            /* se rupe celula de restul listei */
            p->prev->next = p->next;
            p->next->prev = p->prev;

            /* celula de eliminat este prima din lista */
            if (p == *list) {
                /* este singura celula din lista */
                if (p->next == p)
                    *list = NULL;
                else
                    *list = p->next;
            }

            /* se elibereaza memoria ocupata de celula */
            free_data(p->data);
            free(p);

            return 1;
        }

        p = p->next;
    } while (p != *list);

    return 0;
}

/* afiseaza in stream lista data */
void print_list(struct node *list, void (*print_data)(void *, FILE *),
                FILE *stream)
{
    if (list == NULL) {
        fprintf(stream, "VIDA\n");
        return;
    }

    struct node *p = list;

    do {
        print_data(p->data, stream);
        p = p->next;
    } while (p != list);

    fprintf(stream, "\n");
}