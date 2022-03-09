/* CIOBANU Andrei-Bogdan - 312CB */

#include "convert.h"

#define SYMBOLS ".-'?! " /* simbolurile definite in alfabetul pentru trie */
#define SYMBOLS_NUM 6 /* numarul de simboluri din alfabetul pentru trie */

#define LOWERCASE_OFFSET 0 /* alfabetul pentru trie incepe cu literele mici */
#define UPPERCASE_OFFSET 26 /* literele mari incep de la pozitia 26 */
#define DIGITS_OFFSET 52 /* cifrele incep de la pozitia 52 */
#define SYMBOLS_OFFSET 62 /* simbolurile incep de la pozitia 62 */

/* converteste din char in int pentru indexul de alfabet pentru trie */
int char_to_index(char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return LOWERCASE_OFFSET + ch - 'a';

    if (ch >= 'A' && ch <= 'Z')
        return UPPERCASE_OFFSET + ch - 'A';

    if (ch >= '0' && ch <= '9')
        return DIGITS_OFFSET + ch - '0';

    for (int i = 0; i < SYMBOLS_NUM; ++i)
        if (ch == SYMBOLS[i])
            return SYMBOLS_OFFSET + i;

    return -1;
}

/* converteste din int (index de alfabet pentru trie) in char */
char index_to_char(int index)
{
    if (index >= LOWERCASE_OFFSET && index < UPPERCASE_OFFSET)
        return 'a' + index;

    if (index >= UPPERCASE_OFFSET && index < DIGITS_OFFSET)
        return 'A' + index % UPPERCASE_OFFSET;

    if (index >= DIGITS_OFFSET && index < SYMBOLS_OFFSET)
        return '0' + index % DIGITS_OFFSET;

    if (index >= SYMBOLS_OFFSET && index < SYMBOLS_OFFSET + SYMBOLS_NUM)
        return SYMBOLS[index % SYMBOLS_OFFSET];

    return -1;
}