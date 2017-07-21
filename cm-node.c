#include <stdio.h>
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

#define put_char(a)  do {		\
	if (size > 1)			\
		*buf++ = (a), --size;	\
					\
	++total;			\
}					\
while (0)

static size_t cm_print_escaped (const char *p, char *buf, size_t size)
{
	size_t total = 0;

	put_char ('\"');

	for (; *p != '\0'; ++p)
		switch (*p) {
		case '"':
		case '\\':
			put_char ('\\');
			/* passthrough */
		default:
			put_char (*p);
		}

	put_char ('\"');

	if (size > 0)
		*buf = '\0';

	return total;
}

#undef put_char

static size_t get_room (size_t busy, size_t room)
{
	return busy < room ? room - busy : 0;
}

size_t cm_node_print (struct cm_node *o, char *buf, size_t size, int sep)
{
	size_t len, total = 0;

	if (o->root != NULL) {
		len = cm_node_print (o->root, buf, size, sep);
		total += len, buf += len, size = get_room (len, size);

		len = snprintf (buf, size, "%c", sep);
		total += len, buf += len, size = get_room (len, size);
	}

	if (sep != ' ')
		return total + snprintf (buf, size, "%s", o->value);

	return total + cm_print_escaped (o->value, buf, size);
}
