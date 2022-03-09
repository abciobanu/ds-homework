/* CIOBANU Andrei-Bogdan - 312CB */

#include "utils.h"
#include "dns_pair.h"
#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 512
#define DELIMITERS " \n"

/* functia hash pentru perechile de date din memoria unui server DNS */
int dns_hash_func(void *data, size_t buckets_num)
{
    struct dns_pair *pair = data;
    size_t web_address_length = strlen(pair->web_address);
    int chars_sum = 0;

    for (size_t i = 0; i < web_address_length; ++i)
        chars_sum += pair->web_address[i];

    return chars_sum % buckets_num;
}

/* 
 * adauga perechea (web_addr, ip) in tabela hash; daca web_addr exista in tabela
 * hash, aceasta nu va mai fi adaugata
 */
int dns_put(struct hash_table *htable)
{
    char *web_addr = strtok(NULL, DELIMITERS);
    char *ip = strtok(NULL, DELIMITERS);

    /* se creeaza perechea formata din datele anterior extrase */
    struct dns_pair *new_pair = create_pair(web_addr, ip);
    if (new_pair == NULL) {
        fprintf(stderr, "dns_pair memory allocation failed!\n");
        return -1;
    }

    int return_val = add_item(htable, new_pair, cmp_pair);

    if (return_val == 0) {
        /* perechea exista deja in tabela hash */
        free_pair(new_pair);
    } else if (return_val == -1) {
        fprintf(stderr, "New node memory allocation failed!\n");
        return -1;
    }

    return 0;
}

/* 
 * afiseaza in stream adresa IP corespunzatoare web_addr; daca web_addr nu
 * exista in tabela hash, afiseaza "NULL"
 */
int dns_get(struct hash_table *htable, FILE *output_stream)
{
    char *web_addr = strtok(NULL, DELIMITERS);

    /*
     * se creeaza o pereche (web_addr, ""); adresa IP din perechea creata nu
     * are importanta, cautarea facandu-se dupa adresa web
     */
    struct dns_pair *pair_to_be_found = create_pair(web_addr, "");
    if (pair_to_be_found == NULL) {
        fprintf(stderr, "dns_pair memory allocation failed!\n");
        return -1;
    }

    struct dns_pair *result = search_item(htable, pair_to_be_found, cmp_pair);

    if (result == NULL)
        fprintf(output_stream, "NULL\n");
    else
        fprintf(output_stream, "%s\n", result->ip);

    free_pair(pair_to_be_found);

    return 0;
}

/* sterge perechea (web_addr, ip) din tabela hash */
int dns_remove(struct hash_table *htable)
{
    char *web_addr = strtok(NULL, DELIMITERS);

    /*
     * se creeaza o pereche (web_addr, ""); adresa IP din perechea creata nu
     * are importanta, cautarea perechii ce urmeaza a fi eliminata facandu-se
     * dupa adresa web
     */
    struct dns_pair *pair_to_be_removed = create_pair(web_addr, "");
    if (pair_to_be_removed == NULL) {
        fprintf(stderr, "dns_pair memory allocation failed!\n");
        return -1;
    }

    remove_item(htable, pair_to_be_removed, cmp_pair, free_pair);
    free_pair(pair_to_be_removed);

    return 0;
}

/*
 * cauta adresa web in tabela hash si afiseaza in stream "True" sau "False", in
 * functie de caz
 */
int dns_find(struct hash_table *htable, FILE *output_stream)
{
    char *web_addr = strtok(NULL, DELIMITERS);

    /*
     * se creeaza o pereche (web_addr, ""); adresa IP din perechea creata nu
     * are importanta, cautarea facandu-se dupa adresa web
     */
    struct dns_pair *pair_to_be_found = create_pair(web_addr, "");
    if (pair_to_be_found == NULL) {
        fprintf(stderr, "dns_pair memory allocation failed!\n");
        return -1;
    }

    if (existing_item(htable, pair_to_be_found, cmp_pair))
        fprintf(output_stream, "True\n");
    else
        fprintf(output_stream, "False\n");

    free_pair(pair_to_be_found);

    return 0;
}

/*
 * analizeaza datele de intrare, operatii DNS care conduc la operatii asupra
 * tabelei hash: put, get, remove, find, print, print_bucket
 */
int parse_instructions(FILE *input, FILE *output, struct hash_table *htable)
{
    /* buffer pentru liniile din stream-ul de intrare */
    char *line_buffer = malloc(LINE_BUFFER_SIZE);
    if (line_buffer == NULL) {
        fprintf(stderr, "Line buffer memory allocation failed!\n");
        return -1;
    }

    /* se analizeaza fiecare linie din stream */
    while (fgets(line_buffer, LINE_BUFFER_SIZE, input)) {
        /* se imparte linia in token-uri separate de DELIMITERS */
        char *word = strtok(line_buffer, DELIMITERS);
        if (word == NULL)
            break;

        int return_val = 0;

        /* primul token reprezinta tipul instructiunii */
        if (strcmp(word, "put") == 0) {
            return_val = dns_put(htable);
        } else if (strcmp(word, "get") == 0) {
            return_val = dns_get(htable, output);
        } else if (strcmp(word, "remove") == 0) {
            return_val = dns_remove(htable);
        } else if (strcmp(word, "find") == 0) {
            return_val = dns_find(htable, output);
        } else if (strcmp(word, "print") == 0) {
            print_hash_table(htable, print_ip, output);
        } else if (strcmp(word, "print_bucket") == 0) {
            int bucket_index = atoi(strtok(NULL, DELIMITERS));
            print_bucket(htable, bucket_index, print_ip, output);
        }

        /* s-a produs o eroare la una dintre alocarile de memorie */
        if (return_val == -1) {
            free(line_buffer);
            return -1;
        }
    }

    free(line_buffer);

    return 0;
}