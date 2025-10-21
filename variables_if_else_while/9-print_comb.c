#include <stdio.h>

/**
 * main - prints all possible combinations of single-digit numbers
 * Return: Always 0
 */
int main(void)
{
int i;

for (i = 0; i <= 9; i++)
{
putchar(i + 48);  /* Print digit */

if (i != 9)
{
putchar(44);  /* ASCII for ',' */
putchar(32);  /* ASCII for space */
}
}

putchar(10);  /* Newline */

return (0);
}
