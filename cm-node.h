#ifndef CM_NODE_H
#define CM_NODE_H  1

#include <stddef.h>

struct cm_node;

void cm_node_ref   (struct cm_node *o);
void cm_node_unref (struct cm_node *o);

int cm_node_push (struct cm_node **o, const char *value);
char *cm_node_pop (struct cm_node **o);

#endif  /* CM_NODE_H */
