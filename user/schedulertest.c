#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define NFORK 5
#define IO 0

int main() {
  int n, pid;
  int wtime, rtime;
  int twtime=0, trtime=0;
  for (n=0; n < NFORK;n++) {
      pid = fork();
      if (pid < 0)
          break;
      if (pid == 0) {
          if (n < IO) {
            sleep(200); // IO bound processes
          } else {
            for (volatile int i = 0; i < 2000000000; i++) {}; // CPU bound process
          }
          printf("Process %d finished\n", n);
          exit(0);
      } else {
#ifdef LBS
        settickets((10-n)*(10-n)*(10-n));
#endif
#ifdef PBS
        set_priority(60-IO+n, pid); // Will only matter for PBS, set lower priority for IO bound processes
        if (n < IO) {
          set_priority(45-n, pid);
        }
#endif
      }
  }
  for(;n > 0; n--) {
      if(waitx(0,&wtime,&rtime) >= 0) {
          trtime += rtime;
          twtime += wtime;
      } 
  }
  printf("Average rtime %d,  wtime %d\n", trtime / NFORK, twtime / NFORK);
  exit(0);
}