#include "hash_tables.h"
#include <string.h>
#include <stddef.h>

/**
 * hash_table_get - Retrieve the value associated with a key
 * @ht: Pointer to the hash table
 * @key: The key you are looking for
 *
 * Return: The value associated with the key, or NULL if not found
 */
char *hash_table_get(const hash_table_t *ht, const char *key)
{
unsigned long int index;
hash_node_t *node;

if (!ht || !key || strlen(key) == 0)
return (NULL);

index = key_index((const unsigned char *)key, ht->size);

node = ht->array[index];

while (node)
{
if (strcmp(node->key, key) == 0)
return (node->value);
node = node->next;
}

return (NULL);
}
