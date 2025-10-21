#include <stdio.h>

/**
 * main - prints all the numbers of base 16 in lowercase
 * Return: Always 0
 */
int main(void)
{
int i;

for (i = 48; i <= 57; i++)  /* ASCII '0' to '9' */
putchar(i);

for (i = 97; i <= 102; i++)  /* ASCII 'a' to 'f' */
putchar(i);

putchar(10);  /* ASCII newline */

return (0);
}
