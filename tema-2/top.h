/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef TOP_H
#define TOP_H

#include "list.h"
#include "series.h"

/*
 * actualizeaza pozitiile din top, incepand cu celula start; modifier reprezinta
 * valoarea cu care se modifica pozitiile (+1 daca s-a adaugat un serial nou; -1 
 * daca s-a scos un serial din top)
 */
void update_top_positions(struct node *start, int position, int modifier);

/* elimina serialele care au iesit din top10 */
void fix_top_overflow(struct node *start);

/* functia de inserare in lista top10 */
int top_insert(struct node **top, struct series *series);

/*
 * adauga un serial nou in top10; se afiseaza un mesaj de forma:
 * "Categoria top10: [(<nume1>, <rating1>), (<nume2>, <rating2>)].\n"
 */
int add_top(FILE *output, struct node **top);

/* scoate un element din lista; returneaza informatia din celula extrasa */
void *top_remove(struct node **top, struct series *series);

#endif /* TOP_H */
