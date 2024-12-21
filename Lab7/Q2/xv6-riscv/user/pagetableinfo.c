#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("get info\n");
  tell_me(atoi(argv[1]) , atoi(argv[2]));
  exit(0);
}
