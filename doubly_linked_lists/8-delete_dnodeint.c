#include "lists.h"
#include <stdlib.h>

/**
 * delete_dnodeint_at_index - deletes the node at index of a dlistint_t list
 * @head: pointer to the head of the list
 * @index: index of the node to delete (starting at 0)
 * Return: 1 if succeeded, -1 if failed
 */
int delete_dnodeint_at_index(dlistint_t **head, unsigned int index)
{
dlistint_t *current;
unsigned int i;

if (!head || !*head)
return (-1);

current = *head;

if (index == 0)
{
*head = current->next;
if (*head)
(*head)->prev = NULL;
free(current);
return (1);
}

for (i = 0; current && i < index; i++)
current = current->next;

if (!current)
return (-1);

if (current->prev)
current->prev->next = current->next;
if (current->next)
current->next->prev = current->prev;

free(current);
return (1);
}
