/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>

/* reprezentarea unui nod dintr-un arbore de prefixe */
struct trie_node {
    struct trie_node **children;
    void *info;
};

/* creeaza un nod de arbore de prefixe */
struct trie_node *create_trie_node();

/* elibereaza memoria ocupata de un arbore de prefixe */
void free_trie(struct trie_node *trie, void (*free_info)(void *));

/*
 * insereaza o cheie in trie; returneaza adresa pointerului info de la sfarsitul
 * caii
 */
void **trie_insert(struct trie_node *trie, char *key);

/* elimina o cheie din trie; returneaza informatia de la finalul caii */
void *trie_remove(struct trie_node **trie, char *key, size_t level);

/* cauta o cheie si returneaza nodul de la finalul caii */
struct trie_node *trie_search_last_node(struct trie_node *trie, char *key);

/* cauta o cheie in trie; returneaza informatia de la finalul caii */
void *trie_search_info(struct trie_node *trie, char *key);

/*
 * afiseaza cheile din trie; n_max_res = numarul maxim de rezultate de afisat;
 * daca n_max_res = -1, afiseaza toate cheile din trie
 */
void print_trie(struct trie_node *trie, FILE *output, char *string, int level,
                int *n_max_res);

/*
 * functie de autocomplete in trie; afiseaza primele maxim n_max_res rezultate;
 * daca n_max_res = -1, afiseaza toate rezultatele
 */
int trie_autocomplete(struct trie_node *trie, FILE *output, char *prefix,
                      int key_max_len, int n_max_res);

#endif /* TRIE_H */
