#include <kernel.h>
#include <string.h>

#define SCAN_LEFT_ARR 0x4B
#define SCAN_RIGHT_ARR 0x4D
#define SCAN_UP_ARR 0x48
#define SCAN_DOWN_ARR 0x50

// Poll the keyboard for input.
char GetScanCode() {
	char flag = Ports::inb(0x64);
	while (!(flag &1)) {
		flag = Ports::inb(0x64);
	}
	return Ports::inb(0x60);
}

// English-UK Keyboard Layout
unsigned char* scancodes1[0xFF] = {
	"<ESC>",
	"`","1","2","3","4","5","6","7","8","9","0","-","=","\b",
	"\t","q","w","e","r","t","y","u","i","o","p","[","]","\n","<LCTRL>",
	"a","s","d","f","g","h","j","k","l",";","\'","`","<LSHIFT>",
	"\\","z","x","c","v","b","n","m",",",".","/","<RSHIFT>",
	"*","<LALT>"," ","<CAPSL>","","","","","","","","","","","<NUML>"
};

char* getchar() {
	unsigned char c = GetScanCode();

	if (!(c & (1 << 7))) {
		auto scan = scancodes1[c];
		switch (c) {
		case SCAN_LEFT_ARR:
			return "<LARR>";
		case SCAN_RIGHT_ARR:
			return "<RARR>";
		case SCAN_UP_ARR:
			return "<UARR>";
		case SCAN_DOWN_ARR:
			return "<DARR>";
		default:
			return scancodes1[c];
		}
	} else {
		return "";
	}
}