#include <kernel.h>
#include <int.h>

extern "C" void isr_common_stub();
extern "C" void irq_common_stub();
extern "C" void idt_load();


struct idt_entry idt[256];
struct idt_ptr idtp;

/*
 * idt_set_gate
 * Set an IDT gate
 */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,unsigned char flags) {
    idt[num].base_low =     (base & 0xFFFF);
    idt[num].base_high =    (base >> 16) & 0xFFFF;
    idt[num].sel =          sel;
    idt[num].zero =         0;
    idt[num].flags =        flags | 0x60;
}

/* To term.printf the message which defines every exception */
char* exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


/*
 * Remap interrupt handlers
 */
void irq_remap() {
    outportb(0x20, 0x11); /* write ICW1 to PICM, we are gonna write commands to PICM */
    outportb(0xA0, 0x11); /* write ICW1 to PICS, we are gonna write commands to PICS */

    outportb(0x21, 0x20); /* remap PICM to 0x20 (32 decimal) */
    outportb(0xA1, 0x28); /* remap PICS to 0x28 (40 decimal) */

    outportb(0x21, 0x04); /* IRQ2 -> connection to slave */ 
    outportb(0xA1, 0x02);

    outportb(0x21, 0x01); /* write ICW4 to PICM, we are gonna write commands to PICM */
    outportb(0xA1, 0x01); /* write ICW4 to PICS, we are gonna write commands to PICS */

    outportb(0x21, 0x0); /* enable all IRQs on PICM */
    outportb(0xA1, 0x0); /* enable all IRQs on PICS */
}

extern "C" void timer_handler();

void irq_gates() {
    idt_set_gate(32, (unsigned)irq_common_stub, 0x08, 0x8E);//0
    idt_set_gate(33, (unsigned)irq_common_stub, 0x08, 0x8E);//1
    idt_set_gate(34, (unsigned)irq_common_stub, 0x08, 0x8E);//2
    idt_set_gate(35, (unsigned)irq_common_stub, 0x08, 0x8E);//3
    idt_set_gate(36, (unsigned)irq_common_stub, 0x08, 0x8E);//4
    idt_set_gate(37, (unsigned)irq_common_stub, 0x08, 0x8E);//5
    idt_set_gate(38, (unsigned)irq_common_stub, 0x08, 0x8E);//6
    idt_set_gate(39, (unsigned)irq_common_stub, 0x08, 0x8E);//7
    idt_set_gate(40, (unsigned)irq_common_stub, 0x08, 0x8E);//8
    idt_set_gate(41, (unsigned)irq_common_stub, 0x08, 0x8E);//9
    idt_set_gate(42, (unsigned)irq_common_stub, 0x08, 0x8E);//10
    idt_set_gate(43, (unsigned)irq_common_stub, 0x08, 0x8E);//11
    idt_set_gate(44, (unsigned)irq_common_stub, 0x08, 0x8E);//12
    idt_set_gate(45, (unsigned)irq_common_stub, 0x08, 0x8E);//13
    idt_set_gate(46, (unsigned)irq_common_stub, 0x08, 0x8E);//14
    idt_set_gate(47, (unsigned)irq_common_stub, 0x08, 0x8E);//15
}

void irq_install() {
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (unsigned)isr_common_stub, 0x08, 0x8E);
    }

    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uintptr_t)&idt;

    idt_load();

    irq_remap();
    irq_gates();

    //IRQ_RES;
}

void irq_ack(int irq_no) {
    if (irq_no >= 12) {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);
}

extern "C" void fault_handler(struct regs *r) {
    if (r->int_no == 127) {
        term.printf("syscall'd\n");
        return;

    }
    term.printf("exception(0x%x): %s\n", (uint8_t)r->int_no, exception_messages[r->int_no]);

    PAUSE;
}

extern "C" void irq_handler(struct regs *r) {
    IRQ_OFF;
    term.printf("IRQ: %d\n", r->int_no);
    irq_ack(r->int_no - 32);
    IRQ_RES;
}