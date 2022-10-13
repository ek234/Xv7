// setpriority function

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
setpriority(int priority, int pid)
{
  int ret;
  ret = set_priority(priority, pid);
  if (ret < 0) {
    printf("setpriority failed\n");
    exit(1);
  }
  return ret;
}

int
main(int argc, char* argv[])
{
  if ( argc < 3 ) {
    fprintf(2, "Usage: setpriority <priority> <pid>\n");
    exit(1);
  }
  int priority = atoi(argv[1]);
  int pid = atoi(argv[2]);
  setpriority(priority, pid);
  exit(0);
}
