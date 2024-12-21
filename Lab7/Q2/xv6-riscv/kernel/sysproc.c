#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


void give_me(int a , int b){
  struct proc* p = myproc();
  pagetable_t pgt = p->pagetable;
  for (uint64 i = a ; i<=b ; i++){
    uint64 vaddr = i << 12;
    printf("Virtual page number: %ld\n" , i);
    pte_t *pte = walk(pgt , vaddr , 0);
    if (pte == 0){
      printf("Physical frame number: Not available\n");
      printf("\n------------------------------------\n");
      continue;
    }
    if ((*pte & PTE_V) == 0){
      printf("Page Not Valid\n");
      printf("\n------------------------------------\n");
      continue;
    }
    if ((*pte & PTE_U) == 0){
      printf("User Accessible: No\n");
      printf("\n------------------------------------\n");
    }

    printf("Physical page number: %ld\n" , PTE2PA(*pte)>>12);

    int r , w , x;
    if ((*pte & PTE_R) == 0) r = 0;
    else r = 1;
    if ((*pte & PTE_W) == 0) w = 0;
    else w = 1;
    if ((*pte & PTE_X) == 0) x = 0;
    else x = 1;

    printf("R-W-X: %d-%d-%d\n" , r , w , x);
    printf("\n---------------------------------------\n");
  }

  return;
}


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_tell_me(void)
{
  int a;
  int b;
  argint(0 , &a);
  argint(1 , &b);
  give_me(a , b);
  return a + b;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
