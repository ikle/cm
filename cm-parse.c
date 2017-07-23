/*
 * Configuration Management, Parser Helper Module
 *
 * Copyright (c) 2011-2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

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

char *cm_parse (const char *spec, const char *node, char *buf, size_t size)
{
	const size_t len = strlen (node);
	FILE *f;

	if ((f = fopen (spec, "r")) == NULL)
		return NULL;

	while (fgets (buf, size, f) != NULL)
		if (strncmp (buf, node, len) == 0 && buf[len] == ':')
			goto found;

	fclose (f);
	errno = ENOENT;
	return NULL;
found:
	fclose (f);
	return chomp (buf + len + 1);
}
