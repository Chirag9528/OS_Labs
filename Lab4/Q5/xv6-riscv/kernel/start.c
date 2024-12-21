#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"


void main();
void timerinit();

void kernelvec();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// entry.S jumps here in machine mode on stack0.
void
start()
{ 
    
    uint64 x = r_mstatus();
    x|= 1<<3;
    w_mstatus(x);

    uint64 y = r_mie();
    y|= 1<<5;
    w_mie(y);

    w_mtvec((uint64)kernelvec);

    for (int i=0 ; i<10 ; i++){
      threads[i].thread_num = i+1;
    }
    timerinit();
}

// ask each hart to generate timer interrupts.
void
timerinit()
{

  // uint64 x = r_mstatus();
  // x|= 1<<3;
  // w_mstatus(x);

  // uint64 y = r_mie();
  // y|= 1<<5;
  // w_mie(y);

  // w_mtvec((uint64)kernelvec);

  w_stimecmp(r_time() + 10000000);


  // // enable supervisor-mode timer interrupts.
  // w_mie(r_mie() | MIE_STIE);
  
  // // enable the sstc extension (i.e. stimecmp).
  // w_menvcfg(r_menvcfg() | (1L << 63)); 
  
  // // allow supervisor to use stimecmp and time.
  // w_mcounteren(r_mcounteren() | 2);
  
  // // ask for the very first timer interrupt.
  // w_stimecmp(r_time() + 1000000);
}
