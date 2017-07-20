#include <stdio.h>
#include <stdlib.h>

#include "../cm-parse.h"

int main (int argc, char *argv[])
{
	const char *spec, *node;
	char *value;

	if (argc != 3) {
		fprintf (stderr, "usage:\n\tparse-test <spec-file> <node>\n");
		return 1;
	}

	spec = argv[1];
	node = argv[2];

	if ((value = cm_parse (spec, node)) == NULL) {
		perror ("parse-test");
		return 1;
	}

	printf ("%s\n", value);
	free (value);
	return 0;
}
