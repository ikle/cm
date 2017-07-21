#include <stdio.h>

#include "../cm-node.h"

int main (int argc, char *argv[])
{
	struct cm_node *a, *b;

	if (argc < 2) {
		fprintf (stderr, "usage:\n\tnode-base-test node ...\n");
		return 1;
	}

	a = NULL;

	if (!cm_node_push_list (&a, argv + 1)) {
		perror ("cannot push node");
		goto no_a;
	}

	cm_node_ref (a);
	b = a;

	if (!cm_node_push (&b, "b-node")) {
		perror ("cannot push node");
		goto no_b;
	}

	cm_node_unref (a);
	cm_node_unref (b);
	return 0;
no_b:
	cm_node_unref (a);
no_a:
	cm_node_unref (a);
	return 1;
}
