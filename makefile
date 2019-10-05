COMPILER = g++
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding -w -I include -g -fpermissive
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T src/link.ld
EMULATOR = qemu-system-i386
EFLAGS = -m 512 -vga std -display gtk -s -netdev user,id=vmnic -device rtl8139,netdev=vmnic -machine type=q35,accel=kvm -cpu host,check -smp 12 -enable-kvm
KVERSION = alpha-004
OBJS = obj/kmainasm.o obj/kmaincc.o  obj/vgafont.o obj/vga.o obj/system.o obj/pci.o obj/terminal.o obj/acpi.o obj/mem.o obj/irq.o obj/gdt.o obj/drawtest.o obj/input.o obj/ata.o obj/maths.o obj/string.o obj/fpu.o obj/rtl.o
OUTPUT = vos/boot/$(KVERSION).bin

run: all
	@sudo bash ./mkcd.sh
	@#$(EMULATOR) $(EFLAGS) -cdrom vos.iso disk.img -boot d
all:$(OBJS)
	@$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)

obj/kmainasm.o:src/ke/kmain.asm
	@$(ASSEMBLER) $(ASFLAGS) -o obj/kmainasm.o src/ke/kmain.asm
	@echo "[ASSEMBLE]: ke/kmain.asm"

obj/fpu.o:src/ke/fpu.asm
	@$(ASSEMBLER) $(ASFLAGS) -o obj/fpu.o src/ke/fpu.asm
	@echo "[ASSEMBLE]: ke/fpu.asm"

obj/kmaincc.o:src/ke/kmain.cc
	@$(COMPILER) $(CFLAGS) -o obj/kmaincc.o src/ke/kmain.cc
	@echo "[COMPILE]: ke/kmain.cc"

obj/vgafont.o:src/ke/vgafont.cc
	@$(COMPILER) $(CFLAGS) -o obj/vgafont.o src/ke/vgafont.cc
	@echo "[COMPILE]: ke/vgafont.cc"

obj/vga.o:src/ke/vga.cc
	@$(COMPILER) $(CFLAGS) -o obj/vga.o src/ke/vga.cc
	@echo "[COMPILE]: ke/vga.cc"

obj/system.o:src/ke/system.cc
	@$(COMPILER) $(CFLAGS) -o obj/system.o src/ke/system.cc
	@echo "[COMPILE]: ke/system.cc"

obj/pci.o:src/ke/pci.cc
	@$(COMPILER) $(CFLAGS) -o obj/pci.o src/ke/pci.cc
	@echo "[COMPILE]: ke/pci.cc"

obj/terminal.o:src/ke/terminal.cc
	@$(COMPILER) $(CFLAGS) -o obj/terminal.o src/ke/terminal.cc
	@echo "[COMPILE]: ke/terminal.cc"

obj/acpi.o:src/ke/acpi.cc
	@$(COMPILER) $(CFLAGS) -o obj/acpi.o src/ke/acpi.cc
	@echo "[COMPILE]: ke/acpi.cc"

obj/mem.o:src/ke/mem.cc
	@$(COMPILER) $(CFLAGS) -o obj/mem.o src/ke/mem.cc
	@echo "[COMPILE]: ke/mem.cc"

obj/irq.o:src/ke/irq.cc
	@$(COMPILER) $(CFLAGS) -o obj/irq.o src/ke/irq.cc
	@echo "[COMPILE]: ke/irq.cc"

obj/gdt.o:src/ke/gdt.cc
	@$(COMPILER) $(CFLAGS) -o obj/gdt.o src/ke/gdt.cc
	@echo "[COMPILE]: ke/gdt.cc"

obj/drawtest.o:src/ke/drawtest.cc
	@$(COMPILER) $(CFLAGS) -o obj/drawtest.o src/ke/drawtest.cc
	@echo "[COMPILE]: ke/drawtest.cc"

obj/input.o:src/ke/input.cc
	@$(COMPILER) $(CFLAGS) -o obj/input.o src/ke/input.cc
	@echo "[COMPILE]: ke/input.cc"

obj/ata.o:src/ke/ata.cc
	@$(COMPILER) $(CFLAGS) -o obj/ata.o src/ke/ata.cc
	@echo "[COMPILE]: ke/ata.cc"

obj/maths.o:src/ke/maths.cc
	@$(COMPILER) $(CFLAGS) -o obj/maths.o src/ke/maths.cc
	@echo "[COMPILE]: ke/maths.cc"

obj/string.o:src/ke/string.cc
	@$(COMPILER) $(CFLAGS) -o obj/string.o src/ke/string.cc
	@echo "[COMPILE]: ke/string.cc"

obj/rtl.o:src/ke/RTL8139.cc
	@$(COMPILER) $(CFLAGS) -o obj/rtl.o src/ke/RTL8139.cc
	@echo "[COMPILE]: ke/RTL8139.cc"
