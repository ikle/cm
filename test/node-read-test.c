#include <stdio.h>
#include <string.h>

#include "../cm-node.h"

int main (int argc, char *argv[])
{
	const char *conf;
	struct cm_node *n;
	char buf[BUFSIZ];

	if (argc < 2) {
		fprintf (stderr, "usage:\n\tnode-read-test <conf>\n");
		return 1;
	}

	conf = argv[1];
	n = NULL;

	if (!cm_node_read (conf, &n, "interfaces", "ethernet", "eth0",
			   "mtu", "*", NULL)) {
		perror ("cannot read node");
		goto no_read;
	}

	if (cm_node_print (n, buf, sizeof (buf), ' ') >= sizeof (buf)) {
		fprintf (stderr, "buffer overflow\n");
		goto no_print;
	}

	printf ("%s\n", buf);
	cm_node_unref (n);
	return 0;
no_print:
	cm_node_unref (n);
no_read:
	return 1;
}
