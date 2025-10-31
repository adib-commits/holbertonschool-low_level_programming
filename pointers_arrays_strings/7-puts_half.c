#include "main.h"

/**
* puts_half - prints half of a string, followed by a new line
* @str: pointer to the string
*/
void puts_half(char *str)
{
int len = 0;
int n;

/* Find the length of the string */
while (str[len] != '\0')
len++;

/* Calculate the starting index for the second half */
if (len % 2 == 0)
n = len / 2;
else
n = (len + 1) / 2;

/* Print from n to the end of the string */
while (str[n] != '\0')
{
_putchar(str[n]);
n++;
}
_putchar('\n');
}
