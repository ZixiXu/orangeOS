
#ifndef _ORANGES_CONST_H_
#define _ORANGES_CONST_H_

/* EXTERN is defined as "extern" except in global.c, see global.h */
#define EXTERN extern

// function types
#define PUBLIC
#define PRIVATE static

#define TRUE    1
#define FALSE   0

/* COLOR DEFINITIONS 
 * e.g. MAKECOLOR(BLACK | RED) | BRIGHT
 */
#define BLACK   0x0
#define WHITE   0x7
#define RED     0x4
#define GREEN   0x2
#define BLUE    0x1
#define FLASH   0x80
#define BRIGHT  0x08
#define MAKE_COLOR(x, y) (x | y)    /* (background, foreground) */

#define GDT_SIZE        128
#define IDT_SIZE        256

// privilege
#define PRIVILEGE_KRNL  0
#define PRIVILEGE_TASK  1
#define PRIVILEGE_USER  3

// RPL 
#define RPL_KRNL        SA_RPL0
#define RPL_TASK        SA_RPL1
#define RPL_USER        SA_RPL3

/* TTY */
#define NR_CONSOLES    3     /* consoles, 32kb video mem is enough for 3 80*25 consoles */

/* 8259A interrupt controller ports */
#define INT_M_CTL      0x20  /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK  0x21  /* Setting bits in this port disables ints <Master> */
#define INT_S_CTL      0xA0  /* I/O port for 2nd interrupt controller   <Slave>  */
#define INT_S_CTLMASK  0xA1  /* Setting bits in this port disables ints <Slave>  */

/* 8253/8254 PIT (Programmable Interval Timer), see Section 6.5.2.1 */
#define TIMER0         0x40       /* I/O port for timer channel 0 */
#define TIMER_MODE     0x43       /* I/O port for timer mode control */
#define RATE_GENERATOR 0x34       /* 00-11-010-0: Counter 0 - LSB then MSB - mode 2 - bonary, see Figure 6.24 */
#define TIMER_FREQ     1193182L   /* given clock frequency for timer in PC */
#define HZ             100        /* target clock freq we want to set */

/* AT-type keyboard, 8042 ports, see Table 7.1 */
#define KB_DATA        0x60   /* I/O port for kayboard data (read/write) */
#define KB_CMD         0x64   /* I/O port for keyboard command (read status register/write input buffer) */
#define LED_CODE       0xED   /* Command to write to LED lights of keyboard */
#define KB_ACK         0xFA   /* ACK from Keyboard */

/* VGA, see Table 7.4 and 7.5 */
#define CRTC_ADDR_REG  0x3D4   /* Addr Register */
#define CRTC_DATA_REG  0x3D5   /* Data Register */
#define START_ADDR_H   0xC     /* Video mem start addr */
#define START_ADDR_L   0xD     /* Video mem start addr */
#define CURSOR_H       0xE     /* Cursor posision */
#define CURSOR_L       0xF     /* Cursor position */
#define V_MEM_BASE     0xB8000 /* Base of color video mem */
#define V_MEM_SIZE     0x8000  /* 32K */

/* Hardware interrupts */
#define NR_IRQ          16
#define CLOCK_IRQ       0
#define KEYBOARD_IRQ    1
#define CASCADE_IRQ     2      /* cascade to second AT controller */
#define ETHER_IRQ       3      
#define SECONDARY_IRQ   3      /* RS232 int vec for port 2 */ 
#define RS232_IRQ       4      /* RS232 int vec for port 1 */
#define XT_WINI_IRQ     5      /* XT winchester */
#define FLOPPY_IRQ      6
#define PRINTER_IRQ     7
#define AT_WINI_IRQ     14     /* AT winchester */

#define NR_SYS_CALL     2

#endif /* _ORANGES_CONST_H_ */