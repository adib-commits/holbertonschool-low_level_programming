#include "main.h"

/**
* main - Entry point
*
* Return: Always 0 (Success)
*/

int main(void)
{
char phrase[] = "_putchar\n";
int i;

for (i = 0; phrase[i] != '\0'; i++)
{
_putchar(phrase[i]);
}

return (0);
}
