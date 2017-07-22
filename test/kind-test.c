#include <locale.h>
#include <stdio.h>

#include <cm/kind.h>

int main (int argc, char *argv[])
{
	const char *kind, *value;

	setlocale (LC_ALL, "");

	if (argc != 3) {
		fprintf (stderr, "usage:\n\tkind-test <kind> <value>\n");
		return 1;
	}

	kind  = argv[1];
	value = argv[2];

	return cm_kind_validate (kind, value) ? 0 : 1;
}
