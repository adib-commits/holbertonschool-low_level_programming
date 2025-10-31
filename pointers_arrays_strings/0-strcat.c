#include "main.h"

/**
 * _strcat - concatenates the string src to the end of dest
 * @dest: destination string
 * @src: source string to append
 *
 * Return: pointer to dest
 */
char *_strcat(char *dest, char *src)
{
int i = 0, j = 0;

/* Find the end of dest */
while (dest[i] != '\0')
i++;

/* Copy src to the end of dest */
while (src[j] != '\0')
{
dest[i] = src[j];
i++;
j++;
}

dest[i] = '\0'; /* Add null terminator */

return (dest);
}
