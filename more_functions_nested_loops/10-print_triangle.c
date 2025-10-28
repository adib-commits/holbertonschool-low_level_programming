#include "main.h"

/**
 * print_triangle - prints a triangle using the character '#'
 * @size: the size of the triangle
 *
 * Return: void
 */
void print_triangle(int size)
{
int i, j, k;

if (size <= 0)
{
_putchar('\n');
}
else
{
for (i = 1; i <= size; i++)
{
for (j = size - i; j > 0; j--)
_putchar(' ');

for (k = 0; k < i; k++)
_putchar('#');

_putchar('\n');
}
}
}
