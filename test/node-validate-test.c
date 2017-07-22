#include <stdio.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	const char *conf;
	struct cm_node *n;
	char buf[BUFSIZ];

	if (argc < 3) {
		fprintf (stderr, "usage:\n\tnode-validate-test <conf-root>"
				 " <node> ...\n");
		return 1;
	}

	conf = argv[1];
	n = NULL;

	if (!cm_node_push_list (&n, argv + 2)) {
		perror ("cannot push node");
		goto no_node;
	}

	if (!cm_node_validate (conf, n)) {
		cm_node_print (n, buf, sizeof (buf), ' ');
		fprintf (stderr, "%s: syntax error\n", buf);
		goto no_validate;
	}

	cm_node_unref (n);
	return 0;
no_validate:
no_node:
	cm_node_unref (n);
	return 1;
}
