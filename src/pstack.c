#include <stdlib.h>
#include "pstack.h"
#define NIL 0xFFFFFFFFu

static inline uint64_t pack(uint32_t tag, uint32_t idx) { return ((uint64_t)tag << 32) | idx; }
static inline uint32_t idx_of(uint64_t h) { return (uint32_t)h; }
static inline uint32_t tag_of(uint64_t h) { return (uint32_t)(h >> 32); }

static void push_idx(pstack_t *s, _Atomic uint64_t *head, uint32_t i) {
    uint64_t old = atomic_load(head), nw;
    do {
        atomic_store(&s->nodes[i].next, idx_of(old));
        nw = pack(tag_of(old) + 1, i);
    } while (!atomic_compare_exchange_weak(head, &old, nw));
}

static uint32_t pop_idx(pstack_t *s, _Atomic uint64_t *head) {
    uint64_t old = atomic_load(head);
    for (;;) {
        uint32_t i = idx_of(old);
        if (i == NIL) return NIL;
        uint32_t nxt = atomic_load(&s->nodes[i].next);
        uint64_t nw = pack(tag_of(old) + 1, nxt);
        if (atomic_compare_exchange_weak(head, &old, nw)) return i;
    }
}

void pstack_init(pstack_t *s, uint32_t cap) {
    s->nodes = malloc(cap * sizeof(pnode_t));
    atomic_store(&s->data, pack(0, NIL));
    atomic_store(&s->freel, pack(0, NIL));
    for (uint32_t i = cap; i > 0; i--) push_idx(s, &s->freel, i - 1);
}

int pstack_push(pstack_t *s, int v) {
    uint32_t i = pop_idx(s, &s->freel);
    if (i == NIL) return 0;
    s->nodes[i].value = v;
    push_idx(s, &s->data, i);
    return 1;
}

int pstack_pop(pstack_t *s, int *out) {
    uint32_t i = pop_idx(s, &s->data);
    if (i == NIL) return 0;
    *out = s->nodes[i].value;
    push_idx(s, &s->freel, i);
    return 1;
}

void pstack_destroy(pstack_t *s) { free(s->nodes); }
