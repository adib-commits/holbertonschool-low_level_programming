#include "main.h"

/**
 * rev_string - reverses a string
 * @s: pointer to the string to reverse
 *
 * Return: void
 */
void rev_string(char *s)
{
int i = 0;
int j = 0;
char temp;

/* Find the length of the string */
while (s[j] != '\0')
j++;

j--; /* Set j to the last valid index */

/* Swap characters from start and end moving towards the center */
while (i < j)
{
temp = s[i];
s[i] = s[j];
s[j] = temp;
i++;
j--;
}
}
