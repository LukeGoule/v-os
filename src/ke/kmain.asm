bits 32
section .text
%define FL_VIDEO (1 << 2)
%define FLAGS FL_VIDEO 
align 4
	dd 0x1BADB002
	dd FLAGS
	dd - (0x1BADB002+FLAGS)
	dd 0,0,0,0,0,0,1280,720,32

; There is no need for us to setup a GDT or anything
; as GRUB does all the hard work for us =D

global start

global isr_common_stub
global irq_common_stub

extern kmain
extern c_isr_handler

start:
	mov esp, 0x7FFFF
	push esp

	push ebx
	push eax

	cli

	call kmain

; Interrupt Descriptor Table
global idt_load
extern idtp
idt_load:
	lidt [idtp]
	ret

; Interrupt handlers
extern fault_handler
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	; Call the C kernel fault handler
	mov eax, fault_handler

	call eax

	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

extern irq_handler
irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	; Call the C kernel hardware interrupt handler
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

global timer_handler

timer_handler:
	iret


; epic gdt shit
; Global Descriptor Table
global gdt_flush
extern gp
gdt_flush:
	; Load the GDT
	lgdt [gp]
	; Flush the values to 0x10
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2
flush2:
	ret

global tss_flush
tss_flush:
	mov ax, 0x2B
	ltr ax
	ret

; BSS Section
SECTION .bss
	resb 8192 ; 8KB of memory reserved