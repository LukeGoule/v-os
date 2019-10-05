#ifndef KERN_GDT_H
#define KERN_GDT_H

/*
 * Global Descriptor Table Entry
 */
struct gdt_entry {
	/* Limits */
	unsigned short limit_low;
	/* Segment address */
	unsigned short base_low;
	unsigned char base_middle;
	/* Access modes */
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

/*
 * GDT pointer
 */
struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

void gdt_set_gate(int num,
				  unsigned long base,
				  unsigned long limit,
				  unsigned char access,
				  unsigned char gran
				);

void gdt_install();

#endif