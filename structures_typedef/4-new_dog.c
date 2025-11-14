#include <stdlib.h>
#include "dog.h"

/**
 * _strlen - returns the length of a string
 * @s: string
 * Return: length of string
 */
int _strlen(char *s)
{
int i = 0;

while (s[i] != '\0')
i++;
return (i);
}

/**
 * _strcpy - copies a string
 * @dest: destination buffer
 * @src: source string
 * Return: dest
 */
char *_strcpy(char *dest, char *src)
{
int i;

for (i = 0; src[i] != '\0'; i++)
dest[i] = src[i];
dest[i] = '\0';

return (dest);
}

/**
 * new_dog - creates a new dog
 * @name: dog's name
 * @age: dog's age
 * @owner: owner's name
 *
 * Return: pointer to new dog_t, or NULL on failure
 */
dog_t *new_dog(char *name, float age, char *owner)
{
dog_t *dog;
int name_len, owner_len;

dog = malloc(sizeof(dog_t));
if (dog == NULL)
return (NULL);

name_len = _strlen(name);
owner_len = _strlen(owner);

dog->name = malloc(name_len + 1);
if (dog->name == NULL)
{
free(dog);
return (NULL);
}

dog->owner = malloc(owner_len + 1);
if (dog->owner == NULL)
{
free(dog->name);
free(dog);
return (NULL);
}

_strcpy(dog->name, name);
dog->age = age;
_strcpy(dog->owner, owner);

return (dog);
}
