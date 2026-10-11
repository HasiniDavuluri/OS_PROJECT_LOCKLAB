#include <stdlib.h>
#include "stack.h"
void lf_push(lf_stack_t *s, int v) {
    node_t *n = malloc(sizeof *n);
    n->value = v;
    node_t *old = atomic_load(&s->head);
    do { n->next = old; }
    while (!atomic_compare_exchange_weak(&s->head, &old, n));
}
int lf_pop(lf_stack_t *s, int *out) {
    node_t *old = atomic_load(&s->head);
    while (old && !atomic_compare_exchange_weak(&s->head, &old, old->next))
        ;
    if (!old) return 0;
    *out = old->value;   /* not freed: reclamation comes in Week 6 */
    return 1;
}
