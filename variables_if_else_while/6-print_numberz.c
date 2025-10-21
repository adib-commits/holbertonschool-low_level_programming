#include <stdio.h>

/**
 * main - prints all single digit numbers of base 10 starting from 0
 * Return: Always 0
 */
int main(void)
{
    int i;

    for (i = 0; i < 10; i++)
        putchar(i + 48);  /* 48 is ASCII code for '0' */

    putchar(10);  /* 10 is ASCII code for newline '\n' */

    return (0);
}
