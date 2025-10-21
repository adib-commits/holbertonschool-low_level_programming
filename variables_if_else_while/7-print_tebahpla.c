#include <stdio.h>

/**
 * main - prints the lowercase alphabet in reverse
 * Return: Always 0
 */
int main(void)
{
int ch;

for (ch = 122; ch >= 97; ch--)  /* ASCII 'z' = 122, 'a' = 97 */
putchar(ch);

putchar(10);  /* ASCII for newline */

return (0);
}
