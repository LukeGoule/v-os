#include <kernel.h>
#include <string.h>

bool g_bCaps = false; // Is caps lock on?

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

		return scancodes1[c];

	} else {
		return "";
	}
}