/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>

/* reprezentarea unei tabele hash (tabela de dispersie) */
struct hash_table {
    size_t buckets_num;
    int (*hash_func)(void *, size_t);
    struct node **bucket_list;
};

/* 
 * initializeaza o tabela hash de dimensiune buckets_num si cu functia de
 * hashing hash_func
 */
struct hash_table *init_hash_table(size_t buckets_num,
                                   int (*hash_func)(void *, size_t));

/* elibereaza memoria ocupata de tabela hash data */
void free_hash_table(struct hash_table **htable, void (*free_data)(void *));

/* adauga in tabela hash informatia data */
int add_item(struct hash_table *htable, void *item,
             int (*cmp_f)(void *, void *));

/* verifica daca in tabela hash exista informatia data */
int existing_item(struct hash_table *htable, void *item,
                  int (*cmp_f)(void *, void *));

/*
 * cauta in tabela hash informatia data si intoarce adresa acesteia (sau NULL
 * daca nu exista in tabela hash)
 */
void *search_item(struct hash_table *htable, void *item,
                  int (*cmp_f)(void *, void *));

/* elimina din tabela hash informatia data */
int remove_item(struct hash_table *htable, void *item,
                int (*cmp_f)(void *, void *), void (*free_data)(void *));

/* afiseaza in stream bucket-ul cu indice bucket_index */
void print_bucket(struct hash_table *htable, size_t bucket_index,
                  void (*print_data)(void *, FILE *), FILE *stream);

/* afiseaza in stream intreaga tabela hash */
void print_hash_table(struct hash_table *htable,
                      void (*print_data)(void *, FILE *), FILE *stream);

#endif /* HASH_TABLE_H */