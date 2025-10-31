#include "main.h"

/**
* _strncpy - copies a string up to n characters
* @dest: destination buffer
* @src: source string
* @n: maximum number of bytes to copy
*
* Return: pointer to dest
*/
char *_strncpy(char *dest, char *src, int n)
{
int i;

/* Copy characters from src to dest */
for (i = 0; i < n && src[i] != '\0'; i++)
{
dest[i] = src[i];
}

/* If src is shorter than n, pad dest with '\0' */
for (; i < n; i++)
{
dest[i] = '\0';
}

return (dest);
}
