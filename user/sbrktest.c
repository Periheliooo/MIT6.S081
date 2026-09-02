#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  printf("before sbrk\n");
  vmprint();              // 此时在内核中调用 vmprint

  char *p = sbrk(1);

  printf("after sbrk: %p\n", p);
  vmprint();              // 再次调用 vmprint

  exit(0);
}