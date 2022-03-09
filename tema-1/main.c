/* CIOBANU Andrei-Bogdan - 312CB */

#include "hash_table.h"
#include "dns_pair.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#define N_ARGS 4 /* numarul de argumente necesare */

int main(int argc, char *argv[])
{
    if (argc != N_ARGS) {
        fprintf(stderr, "Incorrect number of arguments!\n");
        return 1;
    }

    /*
     * primul argument: numarul de bucket-uri din tabela hash
     * al doilea argument: fisierul de date de intrare
     * al treilea argument: fisierul de date de iesire
     */
    size_t buckets_num = atoi(argv[1]);
    char *input_filename = argv[2];
    char *output_filename = argv[3];

    struct hash_table *htable = init_hash_table(buckets_num, dns_hash_func);
    if (htable == NULL) {
        fprintf(stderr, "Hashtable memory allocation failed!\n");
        return 1;
    }

    FILE *input_file = fopen(input_filename, "rt");
    if (input_file == NULL) {
        free_hash_table(&htable, free_pair);

        fprintf(stderr, "Input file opening failed!\n");
        return 1;
    }

    FILE *output_file = fopen(output_filename, "wt");
    if (output_file == NULL) {
        free_hash_table(&htable, free_pair);
        fclose(input_file);

        fprintf(stderr, "Output file opening / creation failed!\n");
        return 1;
    }

    int return_val = parse_instructions(input_file, output_file, htable);

    fclose(input_file);
    fclose(output_file);

    free_hash_table(&htable, free_pair);

    /* s-a produs o eroare la una dintre alocarile de memorie */
    if (return_val == -1)
        return 1;

    return 0;
}