#include <stdlib.h>
#include "queue.h"
void lf_queue_init(lf_queue_t *q) {
    qnode_t *d = malloc(sizeof *d);
    d->value = 0;
    atomic_store(&d->next, NULL);
    atomic_store(&q->head, d);
    atomic_store(&q->tail, d);
}
void lf_enqueue(lf_queue_t *q, int v) {
    qnode_t *n = malloc(sizeof *n);
    n->value = v;
    atomic_store(&n->next, NULL);
    for (;;) {
        qnode_t *tail = atomic_load(&q->tail);
        qnode_t *next = atomic_load(&tail->next);
        if (tail == atomic_load(&q->tail)) {
            if (next == NULL) {
                qnode_t *exp = NULL;
                if (atomic_compare_exchange_weak(&tail->next, &exp, n)) {
                    atomic_compare_exchange_strong(&q->tail, &tail, n);
                    return;
                }
            } else {
                atomic_compare_exchange_strong(&q->tail, &tail, next); /* help */
            }
        }
    }
}
int lf_dequeue(lf_queue_t *q, int *out) {
    for (;;) {
        qnode_t *head = atomic_load(&q->head);
        qnode_t *tail = atomic_load(&q->tail);
        qnode_t *next = atomic_load(&head->next);
        if (head == atomic_load(&q->head)) {
            if (head == tail) {
                if (next == NULL) return 0;
                atomic_compare_exchange_strong(&q->tail, &tail, next);
            } else {
                int v = next->value;
                if (atomic_compare_exchange_strong(&q->head, &head, next)) {
                    *out = v;
                    return 1;
                }
            }
        }
    }
}
