#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cm-kind.h"
#include "cm-node.h"
#include "cm-parse.h"

struct cm_node {
	long refs;
	char *value;
	struct cm_node *root;
};

void cm_node_ref (struct cm_node *o)
{
	if (o != NULL)
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

int cm_node_push_list (struct cm_node **o, char *argv[])
{
	size_t i;

	for (i = 0; argv[i] != NULL; ++i)
		if (!cm_node_push (o, argv[i]))
			goto rewind;

	return 1;
rewind:
	for (; i > 0; --i)
		free (cm_node_pop (o));

	return 0;
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

	if (sep != ' ' || cm_kind_validate ("name, number", o->value))
		return total + snprintf (buf, size, "%s", o->value);

	return total + cm_print_escaped (o->value, buf, size);
}

static int validate_value (const char *spec, const char *value)
{
	char *kind;
	int ret;

	if ((kind = cm_parse (spec, "kind")) == NULL)
		return cm_kind_validate ("name", value);

	ret = cm_kind_validate (kind, value);
	free (kind);
	return ret;
}

static size_t validate (struct cm_node *o, char *buf, size_t size)
{
	size_t total = strlen (buf), room = size, len;

	if (o->root != NULL) {
		if ((total = validate (o->root, buf, size)) == 0 ||
		    total >= size)
			return 0;

		room = get_room (total, size);

		total += snprintf (buf + total, room, "/");
		room = get_room (total, size);
	}

	len = snprintf (buf + total, room, "%s", o->value);

	if ((total + len) >= size)
		return 0;

	if (access (buf, F_OK) == 0)  /* is regular? */
		return total + len;

	len = snprintf (buf + total, room, "node.spec");

	if ((total + len) >= size || !validate_value (buf, o->value))
		return 0;

	return total + snprintf (buf + total, room, "node.tag");
}

int cm_node_validate (const char *conf, struct cm_node *o)
{
	char buf[BUFSIZ];

	if (snprintf (buf, sizeof (buf), "%s/", conf) >= sizeof (buf))
		return 0;

	return validate (o, buf, sizeof (buf)) > 0;
}
