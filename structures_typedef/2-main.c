#include <stdio.h>
#include "dog.h"

/**
 * main - check the code
 *
 * Return: Always 0.
 */
int main(void)
{
struct dog my_dog;

my_dog.name = "Poppy";
my_dog.age = 3.5;
my_dog.owner = "Bob";
init_dog(&my_dog);
return (0);

void init_dog(struct dog *d, char *name, float age, char *owner);
}
