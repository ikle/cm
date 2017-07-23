#include <stdio.h>
#include <string.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const size_t size = sizeof (buf);
	const char *conf, *s;
	struct cm_node *n;

	if (argc < 2) {
		fprintf (stderr, "usage:\n\tnode-read-test <conf>\n");
		return 1;
	}

	conf = argv[1];
	n = cm_node_init (buf, size);

	if (!cm_node_read (conf, n, "interfaces", "ethernet", "eth0",
			   "mtu", "*", NULL)) {
		perror ("cannot read node");
		return 1;
	}

	if ((s = cm_node_print (n, ' ')) == NULL) {
		fprintf (stderr, "buffer overflow\n");
		return 1;
	}

	printf ("%s\n", s);
	return 0;
}
