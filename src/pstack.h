#ifndef PSTACK_H
#define PSTACK_H
#include <stdint.h>
#include <stdatomic.h>
typedef struct { int value; _Atomic uint32_t next; } pnode_t;
typedef struct {
    pnode_t *nodes;
    _Atomic uint64_t data;    /* head of the stack: (tag << 32) | index */
    _Atomic uint64_t freel;   /* head of the free list */
} pstack_t;
void pstack_init(pstack_t *s, uint32_t capacity);
int  pstack_push(pstack_t *s, int v);     /* 0 if the pool is full */
int  pstack_pop(pstack_t *s, int *out);   /* 0 if empty */
void pstack_destroy(pstack_t *s);
#endif
