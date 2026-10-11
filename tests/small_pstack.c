#include <stdio.h>
#include <pthread.h>
#include "../src/pstack.h"
#define T 4
#define PER 2000
#define TOTAL (T * PER)
pstack_t st;
_Atomic int seen[TOTAL];
_Atomic long got = 0;
void *prod(void *a) {
    long id = (long)a;
    for (int i = 0; i < PER; i++)
        while (!pstack_push(&st, (int)(id * PER + i))) ;
    return NULL;
}
void *cons(void *a) {
    int v;
    while (got < TOTAL)
        if (pstack_pop(&st, &v)) { atomic_fetch_add(&seen[v], 1); got++; }
    return NULL;
}
int main(void) {
    pthread_t p[T], c[T];
    pstack_init(&st, TOTAL);
    for (long i = 0; i < T; i++) pthread_create(&p[i], NULL, prod, (void *)i);
    for (int i = 0; i < T; i++) pthread_create(&c[i], NULL, cons, NULL);
    for (int i = 0; i < T; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < T; i++) pthread_join(c[i], NULL);
    int bad = 0;
    for (int i = 0; i < TOTAL; i++) if (seen[i] != 1) bad++;
    printf("Pooled stack: %d items wrong: %s\n", bad, bad == 0 ? "PASS" : "FAIL");
    pstack_destroy(&st);
    return 0;
}
