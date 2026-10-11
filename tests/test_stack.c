#include <stdio.h>
#include <pthread.h>
#include "../src/stack.h"
#define THREADS 4
#define PER_THREAD 100000
lf_stack_t st;
_Atomic long popped = 0;
void *producer(void *a) { for (int i = 0; i < PER_THREAD; i++) lf_push(&st, i); return NULL; }
void *consumer(void *a) {
    int v;
    while (popped < (long)THREADS * PER_THREAD)
        if (lf_pop(&st, &v)) popped++;
    return NULL;
}
int main(void) {
    pthread_t p[THREADS], c[THREADS];
    for (int i = 0; i < THREADS; i++) pthread_create(&p[i], NULL, producer, NULL);
    for (int i = 0; i < THREADS; i++) pthread_create(&c[i], NULL, consumer, NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < THREADS; i++) pthread_join(c[i], NULL);
    printf("Stack: popped %ld of %d: %s\n", (long)popped, THREADS * PER_THREAD,
           popped == (long)THREADS * PER_THREAD ? "PASS" : "FAIL");
    return 0;
}
