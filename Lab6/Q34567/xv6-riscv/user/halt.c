#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{       
    int pid = getpid();
    printf("%d\n" , pid);
    kill(2);
    kill(1);
}