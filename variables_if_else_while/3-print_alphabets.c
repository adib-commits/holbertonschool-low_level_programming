#include <stdio.h>

/**
 * main - Entry point
 *
 * Description: Prints the alphabet in lowercase, then uppercase
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char ch;

	/* Print lowercase letters */
	for (ch = 'a'; ch <= 'z'; ch++)
		putchar(ch);

	/* Print uppercase letters */
	for (ch = 'A'; ch <= 'Z'; ch++)
		putchar(ch);

	/* Print newline */
	putchar('\n');

	return (0);
}
