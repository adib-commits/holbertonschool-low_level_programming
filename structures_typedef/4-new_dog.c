#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dog.h"

/**
 * new_dog - creates a new dog
 * @name: name of the dog
 * @age: age of the dog
 * @owner: owner of the dog
 *
 * Return: pointer to the new dog, or NULL on failure
 */
dog_t *new_dog(char *name, float age, char *owner)
{
dog_t *dog;
char *dog_name;
char *dog_owner;

if (!name || !owner)
return (NULL);

dog = malloc(sizeof(dog_t));
if (!dog)
return (NULL);

dog_name = malloc(strlen(name) + 1);
if (!dog_name)
{
free(dog);
return (NULL);
}
strcpy(dog_name, name);

dog_owner = malloc(strlen(owner) + 1);
if (!dog_owner)
{
free(dog_name);
free(dog);
return (NULL);
}
strcpy(dog_owner, owner);

dog->name = dog_name;
dog->age = age;
dog->owner = dog_owner;

return (dog);
}
