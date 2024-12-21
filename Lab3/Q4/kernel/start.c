#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void kernelvec();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096];

// entry.S jumps here in machine mode on stack0.
void
start()
{
    w_mstatus(r_mstatus() | (1<<3));
    w_mie(r_mie() | SIE_SEIE);
    w_mtvec((uint64)kernelvec);
    plicinit();
    plicinithart();
    uartinit();
}

