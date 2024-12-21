#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void kernelvec();

extern int devintr();


// set up to take exceptions and traps while in the kernel.
void
trapinithart(void)
{
  w_stvec((uint64)kernelvec);
}


int
devintr()
{
  uint64 scause = r_mcause();

  if(scause == 0x8000000000000009L){
    // this is a supervisor external interrupt, via PLIC.

    // irq indicates which device interrupted.
    int irq = plic_claim();

    if(irq == UART0_IRQ){
      uartintr();
    } 

    
    if(irq)
      plic_complete(irq);

  } 
  return 1;
}

