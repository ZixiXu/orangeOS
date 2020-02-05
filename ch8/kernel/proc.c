/*
 * Over all call flow for system call:
 * User process call get_ticks() (main.c)
 * INT_VECTOR_SYS_CALL in get_ticks() will trigger sys_call() in idt table (kernel.asm)
 * sys_call() calls sys_get_ticks() in sys_call_table (proc.c)
 */
#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

PUBLIC int sys_get_ticks() {
    return ticks;
}

/*
 * Function for scheduling the processes
 */
PUBLIC void schedule() {
    PROCESS* p;
    int      greatest_ticks = 0;

    while (!greatest_ticks) {
        // switch to the process with largest ticks
        for (p = proc_table; p < (proc_table + NR_TASKS + NR_PROCS); p++) {
            if (greatest_ticks < p->ticks) {
                greatest_ticks = p->ticks;
                p_proc_ready = p;
            }
        }
        // if all processes have 0 ticks, 
        // assign the initial ticks to all processes and switch to the largest one
        if (!greatest_ticks) {
            for (p = proc_table; p < (proc_table + NR_TASKS + NR_PROCS); p++) {
                p->ticks = p->priority;
            }
        }
    }
}

/*
 * send/receive msg in kernel <ring 0>
 * function: send or receive
 * src_dest: To/From whom the msg is transferred
 * m: pointer to the msg
 * p: the caller proc
 * return 0 if success
 */
PUBLIC int sys_sendrec(int function, int src_dest, MESSAGE* m, struct proc* p) {
    assert(k_reenter == 0);
    assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
            src_dest == ANY ||
            src_dest == INTERRUPT);

    int ret = 0;
    int caller = proc2pid(p);
    MESSAGE* mla = (MESSAGE*)va2la(caller, m);
    mla->source = caller;

    assert(mla->source != src_dest);

    if (function == SEND) {
        ret = msg_send(p, src_dest, m);
        if (ret != 0) {
            return ret;
        }
    } else if (function == RECEIVE) {
        ret = msg_receive(p, src_dest, m);
        if (ret != 0) {
            return ret;
        }
    } else {
        panic("{sys_sendrec} invalid function: "
              "%d (SEND:%d, RECEIVE:%d).", function, SEND, RECEIVE);
    }

    return 0;
}