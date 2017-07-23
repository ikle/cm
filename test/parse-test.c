#include <stdio.h>
#include <cm/parse.h>

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	const char *spec, *node;
	char *value;

	if (argc != 3) {
		fprintf (stderr, "usage:\n\tparse-test <spec-file> <node>\n");
		return 1;
	}

	spec = argv[1];
	node = argv[2];

	if ((value = cm_parse (spec, node, buf, sizeof (buf))) == NULL) {
		perror ("parse-test");
		return 1;
	}

	printf ("%s\n", value);
	return 0;
}
