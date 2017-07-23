#include <stdio.h>
#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const char *conf, *s;
	struct cm_node *n;

	if (argc < 2) {
		fprintf (stderr, "usage:\n\tnode-write-test <conf>\n");
		return 1;
	}

	conf = argv[1];
	n = cm_node_init (buf, sizeof (buf));

	if (!cm_node_read (conf, n, "interfaces", "ethernet", "eth0",
			   "ttl", NULL) ||
	    !cm_node_push (n, "64")) {
		perror ("cannot set up node");
		return 1;
	}

	if ((s = cm_node_print (n, ' ')) == NULL) {
		fprintf (stderr, "buffer overflow\n");
		return 1;
	}

	printf ("%s: ", s);

	if (!cm_node_write (conf, n)) {
		printf ("write failed\n");
		return 1;
	}

	printf ("written\n");
	return 0;
}
