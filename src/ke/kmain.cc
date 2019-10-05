#include <multiboot.h>
#include <kernel.h>
#include <vga.h>
#include <pci.h>
#include <terminal.h>
#include <acpi.h>
#include <drawtest.h>
#include <input.h>
#include <ata.h>
#include <maths.h>
#include <string.h>
#include <gdt.h>
#include <int.h>
#include <RTL8139.h>

#define MAX_OUT_LEN 512

VGA vga;
Terminal term;
RTC real_time;

extern int start_sym;
extern int end_sym;
 
unsigned char RCMOS(int cadd) {
	Ports::outb(cmos_address, cadd);
	return (Ports::inb(cmos_data));
}

char out[MAX_OUT_LEN];
char prev = 0;

static void* next_address;

void* kmalloc(unsigned int size) {
	if (size == 0) return NULL;

	next_address += size;

	return next_address;
}

void main(multiboot_info* mbi) {
	vga = VGA(mbi, mbi->framebuffer_width, mbi->framebuffer_height);
	term = Terminal(vga);
	next_address = (&end_sym) - (&start_sym);

	// install the GDT
	gdt_install();

	// install interrupts
	irq_install();

	// allow FPU usage (floats and doubles)
	enable_fpu();

	// Initialise data if not done already.
	term.bgcolour = 0x0;
	term.colour = byteColours(255, 0, 255);
	term.cursorX = 0;
	term.cursorY = 0;

	// Clear the frame buffer
	term.clear();

	// power management and stuff
	acpi_init();

	// da pci bus (network, graphics card et al)
	pci_init();


	/*
	if (mbi->mods_count > 0) {
		multiboot_module_t* mods = (multiboot_module_t*)mbi->mods_addr;
		uint32_t* module = (uint32_t*)mods->mod_start;
		BITMAPINFOHEADER* bmp_header = (BITMAPINFOHEADER*)((uint32_t)module + 0x0E);

		for (uint32_t x = 0; x < bmp_header->biWidth; x++) {
			for (uint32_t y = 0; y < bmp_header->biHeight; y++) {
				auto off = (uint32_t)(bmp_header + bmp_header->biSize);
				uint32_t* pPxColour = (uint32_t*)(off + bmp_header->biHeight * y + x);

				vga.glPlotPixel(x,y, *pPxColour);
			}
		}

		term.cursorX = 0;
		term.cursorY = bmp_header->biHeight + 16;
	}*/

	term.printf("VOS - The C++ Operating System Example\n");

	real_time.read_rtc();
	term.printf(" - The current time is: %d:%d\n", real_time.hour, real_time.minute);

	term.Print(">");
	int ccy = term.cursorY;
	int ccx = term.cursorX;
	int outptr = 0;
	
	while (1) {
		ata_device* primary = &ata_primary(1);

		char* c= getchar();
		if (c == "\0" || 
			c[0] == '\0' || 
			(strcmp(c, "<CAPSL>") == 0) ||
			(strcmp(c, "<LSHIFT>") == 0) ||
			(strcmp(c, "<RSHIFT>") == 0) ||
			(strcmp(c, "<LALT>") == 0)) continue;
		term.Print(c);
		if (c[0] != '\n') {
			out[outptr] = (uint8_t*)c[0];
			//out[outptr+1] = 0;
			outptr+=1;
		}

		if (term.cursorX < ccx) {
			out[outptr-1] = '\0';
			outptr -= 2;
		}

		if (term.cursorY != ccy) {
			//term.printf("Len:%d\n", strlen(out));
			//break;
			out[outptr+1] = prev;
			char* cmd = out;
			if (strcmpl(cmd, "help", 4) == 0) {
				term.printf("Available commands:\n");
				term.printf("\ttime\n");
				term.printf("\tata init\n");
				term.printf("\tata stat\n");
				term.printf("\tdrawtest\n");
				term.printf("\tfputest\n");
				term.printf("\tdiv0\n");
				term.printf("\tcls / clear\n");
				
			} else if (strcmpl(cmd, "time", 4) == 0) {
				real_time.read_rtc();
				term.printf("The current time is %d:%d\n", real_time.hour, real_time.minute);
			} else if (strcmpl(cmd, "reboot", 4) == 0) {
				for (;;)
					asm("int3");
			} 
			else if (strcmp(cmd, "ata init") == 0) {
				term.clear();
				term.cursorX = 0;
				term.cursorY = 0;
				ata_init_device(&ata_primary(1));
			} 
			else if (strcmp(cmd, "ata stat") == 0) {
				int status = Ports::inb(ata_primary(1).io_base + ATA_REG_COMMAND);
				ata_wait(&ata_primary(1), 0);
				term.printf("\tDevice status: 0x%x\n", status);
			} 
			else if (strcmpl(cmd, "drawtest", 8) == 0) {
				DoDrawTest();
			} 
			else if (strcmp(cmd, "fputest") == 0) {
				float sintest = sin(45.f);
				float costest = cos(90.f);

				term.printf("sin(45) = %f\n", sintest);
				term.printf("cos(90) = %f\n", costest);
			} 
			else if (strcmp(cmd, "div0") == 0) {
				int i = 0/0;
			}
			else if (strcmp(cmd, "cls") == 0 || strcmp(cmd, "clear") == 0) {
				term.clear();
				term.cursorX = 0;
				term.cursorY = 0;
			}
			else if (strcmp(cmd, "shutdown") == 0){
				acpi_poweroff();
			}
			else {
				term.printf("Command %s not recognised.\n", cmd);
			}

			for (int i = 0; i < MAX_OUT_LEN; i++) {
				out[i] = 0;
			}

			outptr = 0;

			term.Print(">");
			ccy = term.cursorY;
		}

		ccx = term.cursorX;
		prev = c[0];
	}
}

extern "C" void kmain(unsigned int magic_number, multiboot_info* mbi) {
	main(mbi);

	while (1);
}
