/*
 * Prototypes for functions
 */

/* klib.asm */
PUBLIC void    out_byte(u16 port, u8 value);
PUBLIC u8      in_byte(u16 port);
PUBLIC void    disp_str(char* info);
PUBLIC void    disp_color_str(char* info, int color);

/* protect.c */
PUBLIC void    init_prot();
PUBLIC u32     seg2phys(u16 seg);

/* klib.c */
PUBLIC void    delay(int time);

/* kernel.asm */
void restart();

/* main.c */
void TestA();
void TestB();
void TestC();

/* i8259.c */
PUBLIC void    put_irq_handler(int irq, irq_handler handler);
PUBLIC void    spurious_irq(int irq);

/* clock.c */
PUBLIC void    clock_handler(int irq);
PUBLIC void    milli_delay(int milli_sec);
PUBLIC void    init_clock();

/* keyboard.c */
PUBLIC void    init_keyboard();

/* tty.c */
PUBLIC void    task_tty();
PUBLIC void    in_process(TTY* p_tty, u32 key);

/* console.c */
PUBLIC void    out_char(CONSOLE* p_con, char ch);
PUBLIC void    scroll_screen(CONSOLE* p_con, int direction);

/* printf.c */
PUBLIC int     printf(const char *fmt, ...);

/* vsprintf.c */
PUBLIC int     vsprintf(char* buf, const char* fmt, va_list arg);

/* proc.c */
PUBLIC int     sys_get_ticks();
PUBLIC void    schedule();
PUBLIC int     sys_write(char* buf, int len, PROCESS* p_proc);

/* syscall.asm */
PUBLIC void    sys_call();
PUBLIC int     get_ticks();      /* user process call this function */
PUBLIC void    write(char* buf, int len);