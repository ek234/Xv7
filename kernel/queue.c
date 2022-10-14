#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "queue.h"

struct queue*
init_queue(void) {
  struct queue *q = (struct queue*)kalloc();

  for (int i = 0; i < NPROC + 1; i++) {
    q->q[i] = 0;
  }

  q->head = 0;
  q->tail = 0;
  return q;
}

int
enq(struct queue *q, struct proc *p, int cur_q) {
  if (q->tail == q->head - 1) {
    return -1; // queue is full
  }
  q->q[q->tail] = p;
  q->tail = (q->tail + 1) % (NPROC + 1);
  p->in_queue = 1;
  p->cur_q = cur_q;
  p->mlfq_wtime = 0;
  return 0;
}

struct proc*
deq(struct queue *q) {
  if (q->head == q->tail) {
    return 0; // queue is empty
  }
  struct proc *p = q->q[q->head];
  q->head = (q->head + 1) % (NPROC + 1);
  return p;
}

int
is_empty(struct queue *q) {
  return q->head == q->tail;
}

struct proc*
head(struct queue *q) {
  if (q->head == q->tail) {
    return 0; // queue is empty
  }
  return q->q[q->head];
}