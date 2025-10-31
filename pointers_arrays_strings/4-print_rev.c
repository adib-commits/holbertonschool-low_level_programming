#include "main.h"

/**
 * print_rev - prints a string, in reverse, followed by a new line
 * @s: pointer to the string to print
 *
 * Return: void
 */
void print_rev(char *s)
{
int length = 0;

/* Find the length of the string */
while (s[length] != '\0')
length++;

/* Print the string in reverse */
while (length > 0)
{
_putchar(s[length - 1]);
length--;
}

_putchar('\n');
}
