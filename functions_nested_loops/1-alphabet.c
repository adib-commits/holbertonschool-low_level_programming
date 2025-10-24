#include "main.h"

/**
* print_alphabet - prints the lowercase alphabet followed by a new line
*/
void print_alphabet(void)
{
char alphabet = 'a';
for (alphabet = 'a'; alphabet <= 'z'; alphabet++)
{
	_putchar(alphabet);
}
_putchar('\n');
}
