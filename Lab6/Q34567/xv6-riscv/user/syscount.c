#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{       
    int num = syscount();
    printf("%d\n", num);
    exit(0);
}