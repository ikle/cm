#ifndef CM_NODE_H
#define CM_NODE_H  1

#include <stddef.h>

struct cm_node;

void cm_node_ref   (struct cm_node *o);
void cm_node_unref (struct cm_node *o);

int cm_node_push (struct cm_node **o, const char *value);
char *cm_node_pop (struct cm_node **o);

/*
 * The cm_node_push_list pushes NULL-terminated sequence of values into
 * the specified node stack. The main purpose of this function is to
 * create a node stack from program arguments.
 */
int cm_node_push_list (struct cm_node **o, char *argv[]);

/*
 * The cm_node_print function write the node path representation into
 * the supplied buffer using the sep as node separator. If separator is
 * space character then node value will be quoted.
 *
 * Returns number of characters that would have been written (excluding
 * the trailing null byte ('\0') used to end strings) if the size were
 * unlimited.
 *
 * The function do not write more then size bytes including trailing
 * null byte. If the output was truncated due to this limit then the
 * returned value will be equal or more then size bytes.
 */
size_t cm_node_print (struct cm_node *o, char *buf, size_t size, int sep);

#endif  /* CM_NODE_H */
