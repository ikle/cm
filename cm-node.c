#include <stdlib.h>
#include <string.h>

#include "cm-node.h"

struct cm_node {
	long refs;
	char *value;
	struct cm_node *root;
};

void cm_node_ref (struct cm_node *o)
{
	++o->refs;
}

void cm_node_unref (struct cm_node *o)
{
	if (o == NULL || (--o->refs) > 0)
		return;

	cm_node_unref (o->root);

	free (o->value);
	free (o);
}

int cm_node_push (struct cm_node **o, const char *value)
{
	struct cm_node *root = *o, *head;

	if ((head = malloc (sizeof (*head))) == NULL)
		goto no_node;

	if ((head->value = strdup (value)) == NULL)
		goto no_value;

	if (root != NULL)
		cm_node_ref (root);

	head->refs = 1;
	head->root = root;

	*o = head;
	return 1;
no_value:
	free (head);
no_node:
	return 0;
}

char *cm_node_pop (struct cm_node **o)
{
	struct cm_node *head = *o, *root = head->root;
	char *value = head->value;

	cm_node_ref (root);  /* detach root  */
	head->value = NULL;  /* detach value */

	cm_node_unref (head);
	*o = root;
	return value;
}
