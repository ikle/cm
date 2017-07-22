#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <cm/parse.h>

static char *chomp (char *line)
{
	size_t len;

	/* strip leading spaces */
	for (; isspace (line[0]); ++line) {}

	/* strip trailing spaces */
	for (len = strlen (line); len > 0 && isspace (line[len - 1]); --len)
		line[len - 1] = '\0';

	return line;
}

char *cm_parse (const char *spec, const char *node)
{
	const size_t len = strlen (node);
	FILE *f;
	char line[256];

	if ((f = fopen (spec, "r")) == NULL)
		return NULL;

	while (fgets (line, sizeof (line), f) != NULL)
		if (strncmp (line, node, len) == 0 && line[len] == ':')
			goto found;

	fclose (f);
	errno = ENOENT;
	return NULL;
found:
	fclose (f);
	return strdup (chomp (line + len + 1));
}
