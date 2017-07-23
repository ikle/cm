#include <stdio.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const size_t size = sizeof (buf);
	const char *conf;
	struct cm_node *n;
	char text[BUFSIZ];

	if (argc < 3) {
		fprintf (stderr, "usage:\n\tnode-validate-test <conf-root>"
				 " <node> ...\n");
		return 1;
	}

	conf = argv[1];
	n = cm_node_init (buf, size);

	if (!cm_node_push_list (&n, argv + 2)) {
		perror ("cannot push node");
		return 1;
	}

	if (!cm_node_validate (conf, n)) {
		cm_node_print (n, text, sizeof (text), ' ');
		fprintf (stderr, "%s: syntax error\n", text);
		return 1;
	}

	return 0;
}
