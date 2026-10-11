#include <stdio.h>
#include <pthread.h>
#include "../src/queue.h"
#define P 4
#define PER 50000
#define TOTAL (P * PER)
lf_queue_t q;
_Atomic int seen[TOTAL];
_Atomic long got = 0;
void *prod(void *a) {
    long id = (long)a;
    for (int i = 0; i < PER; i++) lf_enqueue(&q, (int)(id * PER + i));
    return NULL;
}
void *cons(void *a) {
    int v;
    while (got < TOTAL)
        if (lf_dequeue(&q, &v)) { atomic_fetch_add(&seen[v], 1); got++; }
    return NULL;
}
int main(void) {
    pthread_t p[P], c[P];
    lf_queue_init(&q);
    for (long i = 0; i < P; i++) pthread_create(&p[i], NULL, prod, (void *)i);
    for (int i = 0; i < P; i++) pthread_create(&c[i], NULL, cons, NULL);
    for (int i = 0; i < P; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < P; i++) pthread_join(c[i], NULL);
    int bad = 0;
    for (int i = 0; i < TOTAL; i++) if (seen[i] != 1) bad++;
    printf("Queue: %d items wrong (lost or duplicated): %s\n", bad, bad == 0 ? "PASS" : "FAIL");
    return 0;
}
