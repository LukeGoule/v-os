#ifndef TERMINAL_H
#define TERMINAL_H

#include <vga.h>

class Terminal {
private:
	VGA v;
public:
	int cursorX = 0, cursorY = 100;
	uint32_t colour = 0xFFFFFF;
	uint32_t bgcolour = 0x0;

	Terminal();
	Terminal(VGA v);
	
	void Print(char* str);
	void PrintChar(char c);
	void printf(const char* format, ...);
	void clear();

	inline void SetColour(uint32_t clr) {this->colour = clr;}
	inline void SetBGColour(uint32_t clr) {this->bgcolour = clr;}
};
#endif