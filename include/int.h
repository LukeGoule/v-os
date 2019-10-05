#ifndef KERN_INTERRUPTS_H
#define KERN_INTERRUPTS_H

/*
 * IDT Entry
 */
struct idt_entry {
    unsigned short base_low;
    unsigned short sel;
    unsigned char zero;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

/*
 * IDT pointer
 */
struct idt_ptr {
    unsigned short limit;
    uintptr_t base;
} __attribute__((packed));

void irq_install();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,unsigned char flags);
void irq_remap();
void irq_gates();
void irq_ack(int irq_no);

#endif