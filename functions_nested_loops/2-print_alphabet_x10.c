#include "main.h"

void print_alphabet_x10(void)
{
int ligne;
char ch;
char alphabet[27] = "abcdefghijklmnopqrstuvwxyz\n";

for (ligne = 0; ligne < 10; ligne++)
{
	for (ch = 0; alphabet[ch] != '\0'; ch++)
		_putchar(alphabet[ch]);
}
}
