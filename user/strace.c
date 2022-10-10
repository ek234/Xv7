// strace program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
strace(int mask, char* command_whole[])
{
  trace(mask);

  exec(command_whole[0], command_whole);

  fprintf(2, "exec %s failed\n", command_whole[0]);
  return 1;
}

int
main(int argc, char* argv[])
{
  if ( argc < 3 ) {
    fprintf(2, "Usage: strace <mask> <command> [args]\n");
    exit(1);
  }
  int mask = atoi(argv[1]);
  strace(mask, &argv[2]);
  exit(0);
}
