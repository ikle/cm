#ifndef CM_NODE_H
#define CM_NODE_H  1

#include <stddef.h>
#include <cm/compiler.h>

struct cm_node *cm_node_init (void *buf, size_t size);

int cm_node_push (struct cm_node *o, const char *value);
char *cm_node_pop (struct cm_node *o);

/*
 * The cm_node_push_list pushes NULL-terminated sequence of values into
 * the specified node stack. The main purpose of this function is to
 * create a node stack from program arguments.
 */
int cm_node_push_list (struct cm_node *o, char *argv[]);

/*
 * The cm_node_read pushes NULL-terminated sequence of values into the
 * specified node stack.
 *
 * If the last value equal * (star character) then it reads real value
 * from the configuration and pushes it on top of stack.
 */
int cm_node_read (const char *conf, struct cm_node *o,
		  const char *node, ...) sentinel;

/*
 * The cm_node_print function returns the node path representation
 * constructed using the sep as node separator. If separator is space
 * character then node value will be quoted.
 *
 * Returns pointer into internal node buffer on success, NULL on buffer
 * overflow.
 */
const char *cm_node_print (struct cm_node *o, int sep);

/*
 * The cm_node_validate function validates the specified node stack
 * against configuration specification.
 *
 * Returns non-zero on success, zero on syntax error or on internal node
 * buffer overflow.
 */
int cm_node_validate (const char *conf, struct cm_node *o);

#endif  /* CM_NODE_H */
