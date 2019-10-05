#ifndef KERN_H
#define KERN_H

#include <system.h>
#include <vga.h>
#include <terminal.h>
#include <time.h>

extern VGA vga;
extern Terminal term;
extern RTC real_time;

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }
#define PAUSE   { asm volatile ("hlt"); }
#define IRQS_ON_AND_PAUSE { asm volatile ("sti\nhlt\ncli"); }

/* Registers */
struct regs {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

typedef struct regs regs_t;

#endif
