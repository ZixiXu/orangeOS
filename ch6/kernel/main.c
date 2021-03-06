#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/*
 * This function is to prepare process table and start a process
 */
PUBLIC int kernel_main() {
    disp_str("-----\"kernel_main\" begins-----\n");

    // prepare the process table for all processes
    TASK*        p_task        = task_table;
    PROCESS*     p_proc        = proc_table;
    char*        p_task_stack  = task_stack + STACK_SIZE_TOTAL;
    u16          selector_ldt  = SELECTOR_LDT_FIRST; 

    int i;
    for (i = 0; i < NR_TASKS; i++) {
        strcpy(p_proc->p_name, p_task->name);       // process name
        p_proc->pid = i;                            // process id

        p_proc->ldt_sel = selector_ldt;

        memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
        p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;  // downgrade DPL to exe on ring1
        memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
        p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;  // downgrade DPL to exe on ring1

        p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // first LDT descriptor
        p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // second LDT descriptor
        p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;

        p_proc->regs.eip = (u32)p_task->initial_eip;     // entry point when iretd, exe process function
        p_proc->regs.esp = (u32) p_task_stack;
        p_proc->regs.eflags = 0x1202;       // IF = 1, IOPL = 1, bit 2 is always 1

        p_task_stack -= p_task->stacksize;
        p_proc++;
        p_task++;
        selector_ldt += 1 << 3;
    }

    proc_table[0].ticks = proc_table[0].priority = 15;
    proc_table[1].ticks = proc_table[1].priority = 5;
    proc_table[2].ticks = proc_table[2].priority = 3;
    
    k_reenter = 0;
    ticks = 0;

    // start the process
    p_proc_ready = proc_table;

    // initialize 8253 PIT
    out_byte(TIMER_MODE, RATE_GENERATOR);
    /* write the value of counter to be (TIMER_FREQ / HZ)
     * this makes the output freq to be 100 (HZ), and so every 10ms will have an interrupt
     */
    out_byte(TIMER0, (u8) (TIMER_FREQ / HZ));         // write lower 8-bit
    out_byte(TIMER0, (u8) ((TIMER_FREQ / HZ) >> 8));  // write higher 8-bit

    put_irq_handler(CLOCK_IRQ, clock_handler);    // register the clock int handler
    enable_irq(CLOCK_IRQ);                        // enable the clock int
    restart();

    while (1) {}
}

/* Simple function of process A
 * This process is run on ring1
 * When kernel starts, it uses restart() to do ring0 -> ring1 thus run TestA()
 * When interrupt happens, suspend TestA and exe int handler, ring1 -> ring0
 * esp0 in tss is prepared, to ensure ring1 -> ring0 is ok (the preparation is done
 * in restart() and the handler: mov dword [tss + TSS3_S_SP0], eax)
 */
void TestA() {
    int i = 0;
    while (1) {
        disp_str("A.");
        milli_delay(10);   // now each delay has 1 tick
    }
}

// Another process B
void TestB() {
    int i = 0x1000;
    while(1) {
        disp_str("B.");
        milli_delay(10);
    }
}

void TestC() {
    int i = 0x2000;
    while(1) {
        disp_str("C.");
        milli_delay(10);
    }
}