/* CIOBANU Andrei-Bogdan - 312CB */

#include "dns_pair.h"

#include <stdlib.h>
#include <string.h>

/* creeaza o pereche (web_addr, ip) */
struct dns_pair *create_pair(char *web_addr, char *ip)
{
    struct dns_pair *new_pair = malloc(sizeof(struct dns_pair));
    if (new_pair == NULL)
        return NULL;

    new_pair->web_address = strdup(web_addr);
    if (new_pair->web_address == NULL) {
        free(new_pair);
        return NULL;
    }

    new_pair->ip = strdup(ip);
    if (new_pair->ip == NULL) {
        free(new_pair->web_address);
        free(new_pair);
        return NULL;
    }

    return new_pair;
}

/* elibereaza memoria ocupata de o pereche */
void free_pair(void *p)
{
    struct dns_pair *pair = p;

    free(pair->web_address);
    free(pair->ip);
    free(pair);
}

/* compara doua perechi dupa web_address (cheie) */
int cmp_pair(void *p1, void *p2)
{
    struct dns_pair *a = p1;
    struct dns_pair *b = p2;

    return strcmp(a->web_address, b->web_address);
}

/* afiseaza IP-ul din pereche in stream */ 
void print_ip(void *pair, FILE *stream)
{
    fprintf(stream, "%s ", ((struct dns_pair *)pair)->ip);
}