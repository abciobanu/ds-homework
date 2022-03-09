/* CIOBANU Andrei-Bogdan - 312CB */

#include "list.h"

#include "stdlib.h"

/* creeaza o celula de lista */
struct node *create_node(void *data)
{
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL)
        return NULL;

    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}

/* elibereaza memoria ocupata de o lista data */
void free_list(struct node **list, void (*free_data)(void *))
{
    if (*list == NULL)
        return;

    struct node *p = *list;
    while (p != NULL) {
        struct node *aux = p;
        p = p->next;

        free_data(aux->data);
        free(aux);
    }

    *list = NULL;
}

/*
 * insereaza informatia data inainte de celula care respecta conditia data;
 * returneaza pozitia la care s-a facut inserarea in lista / -1 in caz de eroare
 */
int insert_before(struct node **list, void *data, int (*fcmp)(void *, void *))
{
    struct node *new_node = create_node(data);
    if (new_node == NULL)
        return -1;

    if (*list == NULL) {
        *list = new_node;
        return 1;
    }

    int position = 1;
    struct node *pre = NULL;
    for (struct node *p = *list; p != NULL; pre = p, p = p->next, ++position) {
        /* se respecta conditia */
        if (fcmp(data, p->data) >= 0) {
            new_node->next = p;

            if (pre == NULL)
                *list = new_node;
            else
                pre->next = new_node;

            return position;
        }
    }

    /*
     * nu s-a gasit celula care respecta conditia data
     * => inserare la final de lista
     */
    pre->next = new_node;

    return position;
}

/* identifica un element din lista; returneaza adresa informatiei gasite */
void *find_data(struct node *list, void *data, int (*fcmp)(void *, void *))
{
    for (; list != NULL; list = list->next)
        if (fcmp(data, list->data) == 0)
            return list->data;

    return NULL;
}

/* scoate un element din lista; returneaza informatia din celula extrasa */
void *remove_node(struct node **list, void *data, int (*fcmp)(void *, void *))
{
    if (*list == NULL)
        return NULL;

    for (struct node *p = *list, *ant = NULL; p != NULL; ant = p, p = p->next) {
        if (fcmp(data, p->data) == 0) {
            if (ant == NULL)
                *list = p->next;
            else
                ant->next = p->next;

            void *return_data = p->data;
            free(p); /* nu mai este nevoie de celula extrasa */

            return return_data;
        }
    }

    return NULL;
}

/* afiseaza elementele unei liste date */
void print_list(FILE *stream, struct node *list,
                void (*print_data)(FILE *, void *))
{
    fprintf(stream, "[");

    for (; list != NULL; list = list->next) {
        print_data(stream, list->data);

        if (list->next != NULL)
            fprintf(stream, ", ");
    }

    fprintf(stream, "].\n");
}
