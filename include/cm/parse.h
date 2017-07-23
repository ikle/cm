#ifndef CM_PARSE_H
#define CM_PARSE_H  1

#include <stddef.h>

/*
 * The cm_parse function find node line in spec file and returns
 * content of it in the supplied buffer.
 */
char *cm_parse (const char *spec, const char *node, char *buf, size_t size);

#endif  /* CM_PARSE_H */
