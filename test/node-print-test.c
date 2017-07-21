#include <stdio.h>
#include <string.h>

#include "../cm-node.h"

int main (int argc, char *argv[])
{
	char **list;
	struct cm_node *n;
	char buf[44];
	size_t len;

	if (argc < 3 || strlen (argv[1]) != 1) {
		fprintf (stderr, "usage:\n\t"
				 "node-print-test <separator> <node> ...\n");
		return 1;
	}

	for (list = argv + 2, n = NULL; *list != NULL; ++list)
		if (!cm_node_push (&n, *list)) {
			perror ("cannot push node");
			goto no_push;
		}

	len = cm_node_print (n, buf, sizeof (buf), argv[1][0]);

	printf ("line[%zu/%zu] = %s\n", len, sizeof (buf), buf);
	cm_node_unref (n);
	return 0;
no_push:
	cm_node_unref (n);
	return 1;
}
