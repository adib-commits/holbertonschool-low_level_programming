#include "main.h"

/**
* print_alphabet_x10 - prints 10 times the alphabet in lowercase
* followed by a new line
*/
void print_alphabet_x10(void)
{
int i;
char ch;
char alphabet[27] = "abcdefghijklmnopqrstuvwxyz\n";

for (i = 0; i < 10; i++)
{
for (ch = 0; alphabet[ch] != '\0'; ch++)
_putchar(alphabet[ch]);
}
}
