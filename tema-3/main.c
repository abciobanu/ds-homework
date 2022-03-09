/* CIOBANU Andrei-Bogdan - 312CB */

#include "utils.h"

#include <stdio.h>

#define N_ARGS 3 /* numarul de argumente necesare */

int main(int argc, char *argv[])
{
    if (argc != N_ARGS) {
        fprintf(stderr, "incorrect number of arguments!\n");
        return 1;
    }

    /*
     * primul argument: numele fisierului de date de intrare
     * al doilea argument: numele fisierului de date de iesire
     */
    char *input_filename = argv[1];
    char *output_filename = argv[2];

    FILE *input_file = fopen(input_filename, "rt");
    if (input_file == NULL) {
        fprintf(stderr, "input file opening failed!\n");
        return 1;
    }

    FILE *output_file = fopen(output_filename, "wt");
    if (output_file == NULL) {
        fclose(input_file);

        fprintf(stderr, "output file opening / creation failed!\n");
        return 1;
    }

    /*
     * acesta este pointerul care va puncta catre zona de memorie cu cei doi
     * pointeri catre T1 si T2 - principalii arbori de prefixe
     */
    struct trie_node **tries;

    int return_val = initialize_tries(&tries);
    if (return_val == -1) {
        fclose(input_file);
        fclose(output_file);

        return 1;
    }

    return_val = parse_instructions(input_file, output_file, tries);

    fclose(input_file);
    fclose(output_file);

    free_tries(tries);

    /* s-a produs o eroare la una dintre alocarile de memorie */
    if (return_val == -1)
        return 1;

    return 0;
}
