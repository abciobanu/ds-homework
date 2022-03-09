/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef UTILS_H
#define UTILS_H

#include "trie.h"

#include <stdio.h>

/* initializeaza cei doi arbori de prefixe pentru gestionarea cartilor */
int initialize_tries(struct trie_node ***tries);

/* elibereaza memoria ocupata de cei doi arbori de prefixe */
void free_tries(struct trie_node **tries);

/* analizeaza operatiile date la intrare */
int parse_instructions(FILE *input, FILE *output, struct trie_node **tries);

#endif /* UTILS_H */