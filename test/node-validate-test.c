#include <stdio.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const char *conf, *s;
	struct cm_node *n;

	if (argc < 3) {
		fprintf (stderr, "usage:\n\tnode-validate-test <conf-root>"
				 " <node> ...\n");
		return 1;
	}

	conf = argv[1];
	n = cm_node_init (buf, sizeof (buf));

	if (!cm_node_push_list (n, argv + 2)) {
		perror ("cannot push node");
		return 1;
	}

	if (cm_node_validate (conf, n))
		return 0;

	if ((s = cm_node_print (n, ' ')) == NULL) {
		fprintf (stderr, "buffer overflow\n");
		return 1;
	}

	fprintf (stderr, "%s: syntax error\n", s);
	return 1;
}
