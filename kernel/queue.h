#include "param.h"

struct queue {
  struct proc* q[NPROC+1];
  int head;
  int tail;
};
