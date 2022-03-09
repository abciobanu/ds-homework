/* CIOBANU Andrei-Bogdan - 312CB */

#ifndef CONVERT_H
#define CONVERT_H

#define ALPHABET_SIZE 68

/* converteste din char in int pentru indexul de alfabet pentru trie */
int char_to_index(char ch);

/* converteste din int (index de alfabet pentru trie) in char */
char index_to_char(int index);

#endif /* CONVERT_H */