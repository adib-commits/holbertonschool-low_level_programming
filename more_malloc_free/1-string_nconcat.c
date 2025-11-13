#include "main.h"
#include <stdlib.h>
#include <string.h>

/**
 * string_nconcat - concatenates s1 and the first n bytes of s2
 * @s1: first string
 * @s2: second string
 * @n: maximum number of bytes from s2 to concatenate
 *
 * Return: pointer to newly allocated concatenated string, or NULL on failure
 */
char *string_nconcat(char *s1, char *s2, unsigned int n)
{
char *result;
unsigned int len1 = 0, len2 = 0, i, total_len;

if (s1)
len1 = strlen(s1);
if (s2)
len2 = strlen(s2);

if (n > len2)
n = len2;

total_len = len1 + n;

result = malloc(sizeof(char) * (total_len + 1));
if (!result)
return (NULL);

for (i = 0; i < len1; i++)
result[i] = s1[i];

for (i = 0; i < n; i++)
result[len1 + i] = s2[i];

result[total_len] = '\0';

return (result);
}
