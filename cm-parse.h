#ifndef CM_PARSE_H
#define CM_PARSE_H  1

/*
 * The cm_parse function find node line in spec file and returns
 * content of it in newly allocated buffer.
 */
char *cm_parse (const char *spec, const char *node);

#endif  /* CM_PARSE_H */
