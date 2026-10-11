# LockLab: Lock-Free Data Structures

A concurrent, lock-free library in C (C11 atomics, POSIX threads).

## Contents
- src/stack.c, queue.c : lock-free stack (Treiber) and queue (Michael-Scott)
- src/pstack.c : ABA-safe stack using a node pool and tagged indices
- tests/ : multi-threaded correctness tests
- bench/ : throughput comparison against a mutex-based stack

## Build and run
gcc -std=c11 -Wall -Wextra -g -pthread src/pstack.c tests/test_pstack.c -o test_pstack
./test_pstack

## Team
Hasini (2500032127), Bhagya (2500031384), Rama Lakshmi (2500030774)

