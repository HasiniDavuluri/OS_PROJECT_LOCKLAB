#ifndef STACK_H
#define STACK_H
#include <stdatomic.h>
typedef struct node { int value; struct node *next; } node_t;
typedef struct { _Atomic(node_t *) head; } lf_stack_t;
void lf_push(lf_stack_t *s, int v);
int  lf_pop(lf_stack_t *s, int *out);
#endif
