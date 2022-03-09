/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef UTILS_H
#define UTILS_H

#include "hash_table.h"

#include <stdio.h>

/* functia hash pentru perechile de date din memoria unui server DNS */
int dns_hash_func(void *data, size_t buckets_num);

/*
 * analizeaza datele de intrare, operatii DNS care conduc la operatii asupra
 * tabelei hash: put, get, remove, find, print, print_bucket
 */
int parse_instructions(FILE *input, FILE *output, struct hash_table *htable);

#endif /* UTILS_H */