/* CIOBANU Andrei-Bogdan - 312CB */

#include "trie.h"
#include "convert.h"

#include <stdlib.h>
#include <string.h>

/* creeaza un nod de arbore de prefixe */
struct trie_node *create_trie_node()
{
    struct trie_node *new_node = malloc(sizeof(struct trie_node));
    if (new_node == NULL)
        return NULL;

    new_node->children = calloc(ALPHABET_SIZE, sizeof(struct trie_node *));
    if (new_node->children == NULL) {
        free(new_node);
        return NULL;
    }

    new_node->info = NULL;

    return new_node;
}

/* elibereaza memoria ocupata de un arbore de prefixe */
void free_trie(struct trie_node *trie, void (*free_info)(void *))
{
    if (trie == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; ++i)
        if (trie->children[i] != NULL)
            free_trie(trie->children[i], free_info);

    if (trie->info != NULL)
        free_info(trie->info);

    free(trie->children);
    free(trie);
}

/*
 * insereaza o cheie in trie; returneaza adresa pointerului info de la sfarsitul
 * caii
 */
void **trie_insert(struct trie_node *trie, char *key)
{
    if (trie == NULL)
        return NULL;

    size_t key_length = strlen(key);
    for (size_t i = 0; i < key_length; ++i) {
        int index = char_to_index(key[i]);

        /* caracterul nu face parte din alfabetul pentru trie */
        if (index == -1)
            return NULL;

        /* insereaza caracterul din cheie daca nu exista deja */
        if (trie->children[index] == NULL) {
            trie->children[index] = create_trie_node();
            if (trie->children[index] == NULL)
                return NULL;
        }

        trie = trie->children[index];
    }

    return &trie->info;
}

/* verifica daca nodul are copii */
int has_children(struct trie_node *node)
{
    for (int i = 0; i < ALPHABET_SIZE; ++i)
        if (node->children[i] != NULL)
            return 1;

    return 0;
}

/* elimina o cheie din trie; returneaza informatia de la finalul caii */
void *trie_remove(struct trie_node **trie, char *key, size_t level)
{
    if (*trie == NULL)
        return NULL;

    /* s-a ajuns la ultimul caracter din cheie */
    if (level == strlen(key)) {
        void *info_p = (*trie)->info;

        /* daca nu este final de cheie valida in trie */
        if (info_p == NULL)
            return NULL;

        /* daca nu este nod terminal, doar elimina informatiile despre carte */
        if (has_children(*trie)) {
            (*trie)->info = NULL;
        } else {
            free((*trie)->children);
            free(*trie);
            *trie = NULL;
        }

        return info_p;
    }

    int index = char_to_index(key[level]);

    /* caracterul nu face parte din alfabetul pentru trie */
    if (index == -1)
        return NULL;

    /* continuarea cheii nu se regaseste in trie */
    if ((*trie)->children[index] == NULL)
        return NULL;

    void *returned_info = trie_remove(&(*trie)->children[index], key,
                                      level + 1);

    /*
     * se elibereaza memoria ocupata de nodul curent numai daca nu exista alte
     * chei care sa depinda de acesta
     */
    if (returned_info && (*trie)->info == NULL && !has_children(*trie)) {
        free((*trie)->children);
        free(*trie);
        *trie = NULL;
    }

    return returned_info;
}

/* cauta o cheie si returneaza nodul de la finalul caii */
struct trie_node *trie_search_last_node(struct trie_node *trie, char *key)
{
    if (trie == NULL)
        return NULL;

    size_t key_length = strlen(key);
    for (size_t i = 0; i < key_length; ++i) {
        int index = char_to_index(key[i]);

        /* caracterul nu face parte din alfabetul pentru trie */
        if (index == -1)
            return NULL;

        if (trie->children[index] == NULL)
            return NULL;

        trie = trie->children[index];
    }

    return trie;
}

/* cauta o cheie in trie; returneaza informatia de la finalul caii */
void *trie_search_info(struct trie_node *trie, char *key)
{
    struct trie_node *last_node = trie_search_last_node(trie, key);

    /* returneaza NULL daca finalul caii nu reprezinta finalul unei chei */
    return (last_node == NULL) ? NULL : last_node->info;
}

/*
 * afiseaza cheile din trie; n_max_res = numarul maxim de rezultate de afisat;
 * daca n_max_res = -1, afiseaza toate cheile din trie
 */
void print_trie(struct trie_node *trie, FILE *output, char *string, int level,
                int *n_max_res)
{
    if (trie == NULL)
        return;

    if (trie->info != NULL) {
        string[level] = '\0';
        fprintf(output, "%s\n", string);
        --(*n_max_res);
    }

    for (int i = 0; i < ALPHABET_SIZE && *n_max_res != 0; ++i) {
        if (trie->children[i] != NULL) {
            string[level] = index_to_char(i);
            print_trie(trie->children[i], output, string, level + 1, n_max_res);
        }
    }
}

/*
 * functie de autocomplete in trie; afiseaza primele maxim n_max_res rezultate;
 * daca n_max_res = -1, afiseaza toate rezultatele
 */
int trie_autocomplete(struct trie_node *trie, FILE *output, char *prefix,
                      int key_max_len, int n_max_res)
{
    /* sir de caractere folosit pentru a forma output-ul de la print_trie() */
    char *string = calloc(key_max_len + 1, sizeof(char)); /* + 1 pentru '\0' */
    if (string == NULL)
        return -1;

    size_t prefix_len = strlen(prefix);
    memcpy(string, prefix, prefix_len - 1); /* nu copiaza '~' */

    struct trie_node *prefix_end = trie_search_last_node(trie, string);
    if (prefix_end == NULL) {
        free(string);
        return 0;
    }

    int n_max_res_before = n_max_res;
    print_trie(prefix_end, output, string, prefix_len - 1, &n_max_res);

    free(string);

    /* nu s-a afisat nimic */
    if (n_max_res == n_max_res_before)
        return 0;

    return 1;
}
