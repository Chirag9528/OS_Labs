#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
    uint64 pid = getpid();
    printf("pid using sysytem call: %ld\n",pid);

    uint64 *p= (uint64*)USYSCALL;
    printf("direct accessing pid: %ld\n",*p);

  exit(0);
}
