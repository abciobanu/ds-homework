/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef CMD_HANDLERS_H
#define CMD_HANDLERS_H

#include "trie.h"

#include <stdio.h>

/* adaugare recomandare carte */
int add_book(struct trie_node **tries);

/* cautare carte */
int search_book(struct trie_node *trie, FILE *output);

/* afisare carti autor */
int list_author(struct trie_node *trie, FILE *output);

/* cautare in cartile unui autor */
int search_by_author(struct trie_node *trie, FILE *output);

/* stergere recomandare */
void delete_book(struct trie_node **tries, FILE *output);

#endif /* CMD_HANDLERS_H */
