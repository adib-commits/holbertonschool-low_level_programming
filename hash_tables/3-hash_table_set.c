#include "hash_tables.h"

/**
 * hash_table_set - Adds or updates a key/value pair in a hash table
 * @ht: Pointer to the hash table
 * @key: Key string (cannot be empty)
 * @value: Value string (must be duplicated)
 *
 * Return: 1 on success, 0 on failure
 */

int hash_table_set(hash_table_t *ht, const char *key, const char *value)
{
unsigned long int index;
hash_node_t *node, *temp;
char *value_copy;
if (!ht || !key || strlen(key) == 0 || !value)
return (0);

index = key_index((const unsigned char *)key, ht->size);
temp = ht->array[index];
while (temp)
{
if (strcmp(temp->key, key) == 0)
{
value_copy = strdup(value);
if (!value_copy)
return (0);
free(temp->value);
temp->value = value_copy;
return (1);
}
temp = temp->next;
}
node = malloc(sizeof(hash_node_t));
if (!node)
return (0);
node->key = strdup(key);
if (!node->key)
{
free(node);
return (0);
}

value_copy = strdup(value);
if (!value_copy)
{
free(node->key);
free(node);
return (0);

}

node->value = value_copy;
node->next = ht->array[index];
ht->array[index] = node;

return (1);
}
