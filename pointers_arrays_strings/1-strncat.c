#include "main.h"

/**
 * _strncat - concatenates at most n bytes of src to the end of dest
 * @dest: destination string
 * @src: source string
 * @n: maximum number of bytes to append from src
 *
 * Return: pointer to dest
 */
char *_strncat(char *dest, char *src, int n)
{
int i = 0, j = 0;

/* Find the end of dest */
while (dest[i] != '\0')
i++;

/* Append at most n characters from src */
while (src[j] != '\0' && j < n)
{
dest[i] = src[j];
i++;
j++;
}

dest[i] = '\0'; /* Null-terminate dest */

return (dest);
}
