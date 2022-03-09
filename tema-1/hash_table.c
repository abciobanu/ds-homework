/* CIOBANU Andrei-Bogdan - 312CB */

#include "hash_table.h"
#include "linked_list.h"

#include <stdlib.h>

/* 
 * initializeaza o tabela hash de dimensiune buckets_num si cu functia de
 * hashing hash_func
 */
struct hash_table *init_hash_table(size_t buckets_num,
                                   int (*hash_func)(void *, size_t))
{
    struct hash_table *htable = malloc(sizeof(struct hash_table));
    if (htable == NULL)
        return NULL;

    htable->bucket_list = calloc(buckets_num, sizeof(struct node *));
    if (htable->bucket_list == NULL) {
        free(htable);
        return NULL;
    }

    htable->buckets_num = buckets_num;
    htable->hash_func = hash_func;

    return htable;
}

/* elibereaza memoria ocupata de tabela hash data */
void free_hash_table(struct hash_table **htable, void (*free_data)(void *))
{
    /* elibereaza fiecare bucket (lista) */
    for (size_t hash = 0; hash < (*htable)->buckets_num; ++hash)
        free_list(&(*htable)->bucket_list[hash], free_data);

    free((*htable)->bucket_list);
    free(*htable);

    *htable = NULL;
}

/* adauga in tabela hash informatia data */
int add_item(struct hash_table *htable, void *item,
             int (*cmp_f)(void *, void *))
{
    int hash = htable->hash_func(item, htable->buckets_num);

    return insert_data(&htable->bucket_list[hash], item, cmp_f);
}

/* verifica daca in tabela hash exista informatia data */
int existing_item(struct hash_table *htable, void *item,
                  int (*cmp_f)(void *, void *))
{
    int hash = htable->hash_func(item, htable->buckets_num);

    return existing_data(htable->bucket_list[hash], item, cmp_f);
}

/*
 * cauta in tabela hash informatia data si intoarce adresa acesteia (sau NULL
 * daca nu exista in tabela hash)
 */
void *search_item(struct hash_table *htable, void *item,
                  int (*cmp_f)(void *, void *))
{
    int hash = htable->hash_func(item, htable->buckets_num);

    return search_data(htable->bucket_list[hash], item, cmp_f);
}

/* elimina din tabela hash informatia data */
int remove_item(struct hash_table *htable, void *item,
                int (*cmp_f)(void *, void *), void (*free_data)(void *))
{
    int hash = htable->hash_func(item, htable->buckets_num);

    return remove_node(&htable->bucket_list[hash], item, cmp_f, free_data);
}

/* afiseaza in stream bucket-ul cu indice bucket_index */
void print_bucket(struct hash_table *htable, size_t bucket_index,
                  void (*print_data)(void *, FILE *), FILE *stream)
{
    if (bucket_index < htable->buckets_num)
        print_list(htable->bucket_list[bucket_index], print_data, stream);
}

/* afiseaza in stream intreaga tabela hash */
void print_hash_table(struct hash_table *htable,
                      void (*print_data)(void *, FILE *), FILE *stream)
{
    for (size_t hash = 0; hash < htable->buckets_num; ++hash) {
        if (htable->bucket_list[hash] == NULL)
            continue;

        fprintf(stream, "%u: ", hash);
        print_list(htable->bucket_list[hash], print_data, stream);
    }
}