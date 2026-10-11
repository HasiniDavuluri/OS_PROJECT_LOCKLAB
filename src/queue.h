#ifndef QUEUE_H
#define QUEUE_H
#include <stdatomic.h>
typedef struct qnode { int value; _Atomic(struct qnode *) next; } qnode_t;
typedef struct { _Atomic(qnode_t *) head; _Atomic(qnode_t *) tail; } lf_queue_t;
void lf_queue_init(lf_queue_t *q);
void lf_enqueue(lf_queue_t *q, int v);
int  lf_dequeue(lf_queue_t *q, int *out);
#endif
