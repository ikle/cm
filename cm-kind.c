/*
 * Configuration Management, Kind Helper Module
 *
 * Copyright (c) 2011-2021 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include <cm/kind.h>
#include <cm/parse.h>

/* compare first term in expression with another term */
static int compare_prefix (const char *expr, const char *term)
{
	for (; *expr == *term; ++expr, ++term)
		if (*term == '\0')
			return 1;

	return *expr == ',' && *term == '\0';
}

static const char *next_term (const char *p)
{
	for (; *p != ','; ++p)
		if (*p == '\0')
			return NULL;

	for (++p; isspace (*p); ++p) {}

	return p;
}

static int is_name (const char *value)
{
	char *locale = setlocale (LC_CTYPE, "C");

	if (!isalpha (value[0]))
		goto error;

	for (++value; *value != '\0'; ++value) {
		if (*value == '-')
			++value;

		if (!isalnum (*value))
			goto error;
	}

	return 1;
error:
	setlocale (LC_CTYPE, locale);
	return 0;
}

static int is_number (const char *value)
{
	if (value[0] == '0')
		return value[1] == '\0';

	for (; *value != '\0'; ++value)
		if (!isdigit (*value))
			return 0;

	return 1;
}

static int is_print (const char *value)
{
	size_t size = strlen (value);
	mbstate_t state;
	wchar_t wc;
	int len;

	memset (&state, 0, sizeof (state));

	for (; *value != '\0'; value += len)
		if ((len = mbrtowc (&wc, value, size, &state)) < 0 ||
		    !iswprint (wc))
			return 0;

	return 1;
}

static const struct map {
	const char *name;
	int (*match) (const char *value);
	const char *help;
} map[] = {
	{
		"name",
		is_name,
		"sequence of an one or more Latin letters, digits and "
		"dashes; a dash shall not be the last character; a dash "
		"shall not be immediately followed by another dash",
	},
	{
		"number",
		is_number,
		"sequence of an one or more digits; the first digit "
		"shall not be zero unless it is a single digit",
	},
	{
		"print",
		is_print,
		"sequence of an any printable characters including space",
	},
	{}
};

int cm_kind_validate (const char *kind, const char *value)
{
	const struct map *p;

	do {
		for (p = map; p->name != NULL; ++p)
			if (compare_prefix (kind, p->name) &&
			    p->match (value))
				return 1;
	}
	while ((kind = next_term (kind)) != NULL);

	return 0;
}

int cm_kind_help (const char *kind, size_t index,
		  const char **name, const char **help)
{
	const struct map *p;

	for (; index > 0; --index, kind = next_term (kind))
		if (kind == NULL)
			return 0;

	for (p = map; p->name != NULL; ++p)
		if (compare_prefix (kind, p->name))
			goto found;

	return 0;
found:
	*name = p->name;
	*help = p->help;
	return 1;
}
