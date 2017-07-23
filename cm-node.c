#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cm/kind.h>
#include <cm/node.h>
#include <cm/parse.h>

struct item {
	struct item *parent;
	char value[];
};

struct cm_node {
	struct item *tail;
	char *end;
	struct item item[];
};

static void *align (void *p)  /* align pointer */
{
	const size_t size = sizeof (void *);
	intptr_t a = (intptr_t) p;

	return (void *) (((a + (size - 1)) / size) * size);
}

struct cm_node *cm_node_init (void *buf, size_t size)
{
	struct cm_node *n = align (buf);

	if (size < (sizeof (*n) + sizeof (n->item[0])))
		return NULL;

	n->tail = n->item;
	n->end  = buf + size;

	n->item[0].parent = NULL;

	return n;
}

static struct item *next_item (struct item *i, const char *value)
{
	return align (i->value + strlen (value) + 1);
}

int cm_node_push (struct cm_node *n, const char *value)
{
	struct item *i = n->tail;
	struct item *tail = next_item (i, value);

	if (tail->value > n->end)
		return 0;

	n->tail = tail;
	strcpy (i->value, value);

	return 1;
}

const char *cm_node_pop (struct cm_node *n)
{
	struct item *i = n->tail;

	if (i == NULL)
		return NULL;

	n->tail = i->parent;
	return i->value;
}

int cm_node_push_list (struct cm_node *o, char *argv[])
{
	struct item *tail = o->tail;
	size_t i;

	for (i = 0; argv[i] != NULL; ++i)
		if (!cm_node_push (o, argv[i]))
			goto rewind;

	return 1;
rewind:
	o->tail = tail;  /* rewind: remove incomplete path */
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

static size_t print (struct cm_node *o, char *buf, size_t size, int sep)
{
	struct item *i;
	size_t len, total = 0;

	for (i = o->item; i < o->tail; i = next_item (i, i->value)) {
		if (i != o->item) {
			len = snprintf (buf, size, "%c", sep);
			total += len, buf += len, size = get_room (len, size);
		}

		if (sep != ' ' || cm_kind_validate ("name, number", i->value))
			len = snprintf (buf, size, "%s", i->value);
		else
			len = cm_print_escaped (i->value, buf, size);

		total += len, buf += len, size = get_room (len, size);
	}

	return total;
}

const char *cm_node_print (struct cm_node *o, int sep)
{
	const size_t size = o->end - o->tail->value;

	if (print (o, o->tail->value, size, sep) >= size)
		return NULL;

	return o->tail->value;
}

static int validate_value (const char *spec, const char *value)
{
	char buf[32];
	char *kind;
	int ret;

	if ((kind = cm_parse (spec, "kind", buf, sizeof (buf))) == NULL)
		return cm_kind_validate ("name", value);

	ret = cm_kind_validate (kind, value);
	return ret;
}

static int validate (struct cm_node *o, char *buf, size_t size)
{
	struct item *i;
	size_t total = strlen (buf), room = size, len;

	for (i = o->item; i < o->tail; i = next_item (i, i->value)) {
		len = snprintf (buf + total, room, "%s/", i->value);

		if ((total + len) >= size)
			return 0;

		if (access (buf, F_OK) == 0) {  /* is regular? */
			total += len, room = get_room (total, size);
			continue;
		}

		len = snprintf (buf + total, room, "node.spec");

		if ((total + len) >= size || !validate_value (buf, i->value))
			return 0;

		len = snprintf (buf + total, room, "node.tag/");
		total += len, room = get_room (total, size);
	}

	return 1;
}

int cm_node_validate (const char *conf, struct cm_node *o)
{
	const size_t size = o->end - o->tail->value;

	if (snprintf (o->tail->value, size, "%s/", conf) >= size)
		return 0;

	return validate (o, o->tail->value, size);
}

static char *file_read_all (const char *path)
{
	FILE *f;
	long size;
	char *value;

	if ((f = fopen (path, "rb")) == NULL)
		goto no_file;

	if (fseek (f, 0, SEEK_END) != 0 || (size = ftell (f)) < 0)
		goto no_size;

	rewind (f);

	if ((value = malloc (size + 1)) == NULL)
		goto no_value;

	if (fread (value, size, 1, f) != 1)
		goto no_read;

	fclose (f);
	value[size] = '\0';
	return value;
no_read:
	free (value);
no_value:
no_size:
	fclose (f);
no_file:
	return NULL;
}

static char *read_value (const char *conf, struct cm_node *o)
{
	const size_t size = o->end - o->tail->value;
	char *buf = o->tail->value;

	size_t total, room;
	char *value;
	size_t len;

	total = snprintf (buf, size, "%s/", conf);
	room = get_room (total, size);

	total += print (o, buf + total, room, '/');
	room = get_room (total, size);

	total += snprintf (buf + total, room, "/node.val");

	if (total >= size)
		return NULL;

	if ((value = file_read_all (buf)) == NULL)
		return NULL;

	for (len = strlen (value); len > 0 && isspace (value[len - 1]); --len)
		value[len - 1] = '\0';

	return value;
}

int cm_node_read (const char *conf, struct cm_node *o,
		  const char *node, ...)
{
	va_list ap;
	struct item *tail = o->tail;
	char *value;

	va_start (ap, node);

	for (; node != NULL; node = va_arg (ap, const char *)) {
		if (strcmp (node , "*") == 0) {
			if (va_arg (ap, const char *) != NULL ||
			    (value = read_value (conf, o)) == NULL)
				goto no_node;

			if (!cm_node_push (o, value))
				goto no_value;

			free (value);
			break;
		}

		if (!cm_node_push (o, node))
			goto no_node;
	}

	va_end (ap);
	return 1;
no_value:
	free (value);
no_node:
	o->tail = tail;  /* rewind: remove incomplete path */
	va_end (ap);
	return 0;
}
