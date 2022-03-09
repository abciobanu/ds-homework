/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef DATABASE_OPS_H
#define DATABASE_OPS_H

#include "list.h"
#include "queue.h"
#include "stack.h"
#include "top.h"

#include <stdio.h>

#define DELIMITERS " \n"
#define N_CATEGORIES 4 /* numarul de categorii (inclusiv top10) */

/*
 * adauga un serial nou in categoria data la intrare (1-3); se afiseaza un mesaj
 * de forma: "Serialul <nume> a fost adaugat la pozitia %d.\n"
 */
int add_series(FILE *output, struct node **categories);

/*
 * adauga un sezon nou pentru serialul cu numele name; se afiseaza un mesaj de
 * forma: "Serialul <nume> are un sezon nou.\n"
 */
int add_season(FILE *output,
               struct node **categories,
               struct queue *watch_later,
               struct stack *currently_watching);

/*
 * muta un serial din cele 4 categorii in coada watch_later; afiseaza un mesaj
 * de forma: "Serialul <nume> se afla in coada de asteptare pe pozitia %d.\n"
 */
int later(FILE *output, struct node **categories, struct queue *watch_later);

/*
 * muta un serial in stiva currently_watching daca nu exista deja, apoi
 * vizioneaza un numar de minute dintr-un serial (informatii date la intrare);
 * se afiseaza un mesaj de forma: "Serialul <nume> a fost vizionat integral.\n"
 * in cazul in care serialul a fost vizionat integral
 */
int watch(FILE *output,
          struct node **categories,
          struct queue *watch_later,
          struct stack *currently_watching,
          struct stack *history);

/*
 * afiseaza lista cu serialele din categoria data la intrare, sub forma:
 * "Categoria <X>: [(<numeA>, <rA>), (<numeB>, <rB>), ...].\n"
 */
void show(FILE *output,
          struct node **categories,
          struct queue *watch_later,
          struct stack *currently_watching,
          struct stack *history);

#endif /* DATABASE_OPS_H */
