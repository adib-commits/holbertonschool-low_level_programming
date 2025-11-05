#include "main.h"

/**
 * _sqrt_helper - helper function to find natural square root recursively
 * @n: number to find the square root of
 * @i: current guess
 *
 * Return: the natural square root or -1 if it doesn't exist
 */
int _sqrt_helper(int n, int i)
{
if (i * i == n)
return (i);
if (i * i > n)
return (-1);
return (_sqrt_helper(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number
 * @n: number to calculate the square root of
 *
 * Return: natural square root, or -1 if none exists
 */
int _sqrt_recursion(int n)
{
if (n < 0)
return (-1);
return (_sqrt_helper(n, 0));
}
