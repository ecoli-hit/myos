#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sched.h>

void
main(int argc, char *argv[])
{
  int p1[2];
  pipe(p1);

  int p2[2];
  pipe(p2);

  int pid = fork();

  if (pid == 0){
    /* child thread 
        p1 read
        p2 write*/
    close(p1[1]);
    close(p2[0]);

    char buf[4] ;
    read(p1[0],buf,4);
    printf("%d: received ",getpid());
    printf(buf);
    printf("\n");
    write(p2[1],argv[0]+4,4);
  } else{
    /* father thread 
        p1 write
        p2 raed*/
    close(p1[0]);
    close(p2[1]);

    write(p1[1],argv[0],4);
    char buf[4] ;
    read(p2[0],buf,4);
    printf("%d: received ",getpid());
    printf(buf);
    printf("\n");
  }

  exit(0);

}