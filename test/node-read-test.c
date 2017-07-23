#include <stdio.h>
#include <string.h>

#include <cm/node.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const size_t size = sizeof (buf);
	const char *conf;
	struct cm_node *n;
	char text[BUFSIZ];

	if (argc < 2) {
		fprintf (stderr, "usage:\n\tnode-read-test <conf>\n");
		return 1;
	}

	conf = argv[1];
	n = cm_node_init (buf, size);

	if (!cm_node_read (conf, &n, "interfaces", "ethernet", "eth0",
			   "mtu", "*", NULL)) {
		perror ("cannot read node");
		return 1;
	}

	if (cm_node_print (n, text, sizeof (text), ' ') >= sizeof (text)) {
		fprintf (stderr, "buffer overflow\n");
		return 1;
	}

	printf ("%s\n", text);
	return 0;
}
