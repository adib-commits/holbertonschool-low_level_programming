#ifndef DOG_H
#define DOG_H

/**
 * struct dog - a dog's basic info
 * @name: the name of the dog
 * @age: the age of the dog
 * @owner: the owner of the dog
 */
typedef struct dog
{
char *name;
float age;
char *owner;
} dog_t;

dog_t *new_dog(char *name, float age, char *owner);

#endif
