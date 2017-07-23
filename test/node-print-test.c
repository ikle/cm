#include <stdio.h>
#include <string.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const size_t size = sizeof (buf);
	struct cm_node *n;
	const char *s;

	if (argc < 3 || strlen (argv[1]) != 1) {
		fprintf (stderr, "usage:\n\t"
				 "node-print-test <separator> <node> ...\n");
		return 1;
	}

	n = cm_node_init (buf, size);

	if (!cm_node_push_list (n, argv + 2)) {
		perror ("cannot push node");
		return 1;
	}

	if ((s = cm_node_print (n, argv[1][0])) == NULL) {
		fprintf (stderr, "buffer overflow\n");
		return 1;
	}

	printf ("line = %s\n", s);
	return 0;
}
