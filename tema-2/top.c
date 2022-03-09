/* CIOBANU Andrei-Bogdan - 312CB */

#include "top.h"
#include "get.h"

#include <stdlib.h>

#define TOP_SIZE 10 /* dimensiunea topului (10 seriale) */

/*
 * actualizeaza pozitiile din top, incepand cu celula start; modifier reprezinta
 * valoarea cu care se modifica pozitiile (+1 daca s-a adaugat un serial nou; -1
 * daca s-a scos un serial din top)
 */
void update_top_positions(struct node *start, int position, int modifier)
{
    struct node *current = start;
    for (; current != NULL; current = current->next, position += modifier)
        ((struct series *)current->data)->id = position;
}

/* elimina serialele care au iesit din top10 */
void fix_top_overflow(struct node *start)
{
    struct node *current = start->next;
    struct node *pre = start;
    for (; current->next != NULL; pre = current, current = current->next);

    /* se verifica daca ultimul serial din lista se afla in afara topului */
    if (((struct series *)current->data)->id > TOP_SIZE) {
        free_series(current->data);
        free(current);

        pre->next = NULL;
    }
}

/* functia de inserare in lista top10 */
int top_insert(struct node **top, struct series *series)
{
    struct node *new_node = create_node(series);
    if (new_node == NULL)
        return -1;

    if (*top == NULL) {
        *top = new_node;
        return 1;
    }

    unsigned int position = 1;
    struct node *pre = NULL;
    for (struct node *p = *top; p != NULL; pre = p, p = p->next, ++position) {
        /* daca s-a gasit pozitia de inserare */
        if (series->id == position) {
            new_node->next = p;

            if (pre == NULL)
                *top = new_node;
            else
                pre->next = new_node;

            /* se actualizeaza poziitile (si serialele) din top */
            update_top_positions(p, position + 1, 1);
            fix_top_overflow(new_node);

            return position;
        }
    }

    /* insereaza la sfarsitul listei */
    pre->next = new_node;

    return position;
}

/*
 * adauga un serial nou in top10; se afiseaza un mesaj de forma:
 * "Categoria top10: [(<nume1>, <rating1>), (<nume2>, <rating2>)].\n"
 */
int add_top(FILE *output, struct node **top)
{
    /* campul ID al serialul reprezinta pozitia in top10 */
    struct series *new_series = get_series_from_input();
    if (new_series == NULL)
        return -1;

    int position = top_insert(top, new_series);
    if (position == -1) {
        free_series(&new_series);

        fprintf(stderr, "list insertion failed!\n");
        return -1;
    }

    fprintf(output, "Categoria top10: ");
    print_list(output, *top, print_series);

    return 1;
}

/* scoate un element din lista; returneaza informatia din celula extrasa */
void *top_remove(struct node **top, struct series *series)
{
    if (*top == NULL)
        return NULL;

    unsigned int position = 1;
    struct node *pre = NULL;
    for (struct node *p = *top; p != NULL; pre = p, p = p->next, ++position) {
        /* daca s-a gasit elementul de eliminat */
        if (cmp_series(series, p->data) == 0) {
            if (pre == NULL)
                *top = p->next;
            else
                pre->next = p->next;

            /* se actualizeaza pozitiile din top */
            update_top_positions(p, position, -1);

            void *return_data = p->data;
            free(p);

            return return_data;
        }
    }

    return NULL;
}
