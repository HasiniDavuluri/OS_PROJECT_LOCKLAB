#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../src/stack.h"
#define OPS 200000
typedef struct mnode { int v; struct mnode *next; } mnode_t;
mnode_t *mhead = NULL;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
lf_stack_t lfs;
void *mutex_worker(void *a) {
    for (int i = 0; i < OPS; i++) {
        mnode_t *n = malloc(sizeof *n); n->v = i;
        pthread_mutex_lock(&mtx); n->next = mhead; mhead = n; pthread_mutex_unlock(&mtx);
        pthread_mutex_lock(&mtx); mnode_t *p = mhead; if (p) mhead = p->next; pthread_mutex_unlock(&mtx);
        free(p);
    }
    return NULL;
}
void *lf_worker(void *a) {
    int v;
    for (int i = 0; i < OPS; i++) { lf_push(&lfs, i); lf_pop(&lfs, &v); }
    return NULL;
}
double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec / 1e9; }
double run(void *(*f)(void *), int n) {
    pthread_t th[64];
    double s = now();
    for (int i = 0; i < n; i++) pthread_create(&th[i], NULL, f, NULL);
    for (int i = 0; i < n; i++) pthread_join(th[i], NULL);
    return now() - s;
}
int main(void) {
    int counts[] = {1, 2, 4, 8};
    printf("Threads   Mutex ops/sec   Lock-free ops/sec\n");
    for (int k = 0; k < 4; k++) {
        int n = counts[k];
        double tm = run(mutex_worker, n), tl = run(lf_worker, n);
        printf("%-9d %-15.0f %-15.0f\n", n, 2.0 * OPS * n / tm, 2.0 * OPS * n / tl);
    }
    return 0;
}
