/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef DNS_PAIR_H
#define DNS_PAIR_H

#include <stdio.h>

/* reprezentarea perechilor de date din memoria unui server DNS */
struct dns_pair {
    char *web_address;
    char *ip;
};

/* creeaza o pereche (web_addr, ip) */
struct dns_pair *create_pair(char *web_addr, char *ip);

/* elibereaza memoria ocupata de o pereche */
void free_pair(void *p);

/* compara doua perechi dupa web_address (cheie) */
int cmp_pair(void *p1, void *p2);

/* afiseaza IP-ul din pereche in stream */
void print_ip(void *pair, FILE *stream);

#endif /* DNS_H */