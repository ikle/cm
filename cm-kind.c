#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include <regex.h>

#include "cm-kind.h"
#include "cm-parse.h"

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

static int match (const char *re, const char *value)
{
	regex_t preg;
	int ret;

	if (regcomp (&preg, re, REG_EXTENDED | REG_NOSUB) != 0)
		return 0;

	ret = regexec (&preg, value, 0, NULL, 0) == 0;

	regfree (&preg);
	return ret;
}

static int is_print (const char *cookie, const char *value)
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
	const char *name, *cookie;
	int (*match) (const char *cookie, const char *value);
} map[] = {
	{
		"name",
		"^[A-Za-z](-?[0-9A-Za-z])*$",
		match,
	},
	{
		"number",
		"^(0|([1-9][0-9]*))$",
		match,
	},
	{
		"print",
		NULL,
		is_print,
	},
	{}
};

int cm_kind_validate (const char *kind, const char *value)
{
	const struct map *p;

	do {
		for (p = map; p->name != NULL; ++p)
			if (compare_prefix (kind, p->name) &&
			    p->match (p->cookie, value))
				return 1;
	}
	while ((kind = next_term (kind)) != NULL);

	return 0;
}
