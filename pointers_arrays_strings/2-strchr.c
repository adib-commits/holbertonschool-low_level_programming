#include "main.h"
#include <stddef.h>

/**
 * _strchr - locates a character in a string
 * @s: string to search
 * @c: character to locate
 *
 * Return: pointer to the first occurrence of c in s,
 * or NULL if the character is not found
 */
char *_strchr(char *s, char c)
{
int i;

for (i = 0; s[i] != '\0'; i++)
{
if (s[i] == c)
return (s + i);
}

if (s[i] == c) /* check for the null terminator match */
return (s + i);

return (NULL);
}
