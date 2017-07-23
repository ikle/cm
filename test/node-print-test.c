#include <stdio.h>
#include <string.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	struct cm_node *n;
	char text[38];
	size_t len;

	if (argc < 3 || strlen (argv[1]) != 1) {
		fprintf (stderr, "usage:\n\t"
				 "node-print-test <separator> <node> ...\n");
		return 1;
	}

	n = NULL;

	if (!cm_node_push_list (&n, argv + 2)) {
		perror ("cannot push node");
		goto no_push;
	}

	len = cm_node_print (n, text, sizeof (text), argv[1][0]);

	printf ("line[%zu/%zu] = %s\n", len, sizeof (text), text);
	cm_node_unref (n);
	return 0;
no_push:
	cm_node_unref (n);
	return 1;
}
