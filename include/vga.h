#ifndef VGA_H
#define VGA_H

#include <multiboot.h>
#include <system.h>

#define bit(x, y) ((x >> y) & 0x01)

#define VGA_WIDTH 	1280	// Width of the frame buffer
#define VGA_HEIGHT 	720		// Height of the frame buffer
#define VGA_BPP 	32		// Bits per pixel

// Safe putpixel func
#define _putpixel(v, x,y) v.glPlotPixel(x,y,0xFFFFFF)

// Not so safe everywhere
#define putpixel(x,y) _putpixel(vga, x, y);

// Convert rgb values to an integer
#define byteColours(bR, bG, bB) (((uint8_t)bR << 0) | ((uint8_t)bG << 8) | ((uint8_t)bB << 16)) + 0xFF000000

// Convert rgba values to an integer.
#define byteColoursA(bR, bG, bB, bA) (((uint8_t)bR << 0) | ((uint8_t)bG << 8) | ((uint8_t)bB << 16) | ((uint8_t)bA << 24))


/*
	Terrible implementation of drawing to the screen using the frame buffer.
	Some functions begin with 'gl' to imply they're in a graphics library, this
	was stolen from OpenGL if you couldn't tell. This entire class is bad because it's
	a class in an operating system. Technically there isn't anything wrong with that
	but it's unconventional and makes the code a little bit more difficult to deal
	with IMO.
*/
class VGA {
public:
	void* fb;				// Frame buffer memory address
	multiboot_info* mbi;	// Pointer to the multiboot info structure
	
	uint32_t w;
	uint32_t h;

	inline VGA() {}			// Incredibly bad code

	VGA(multiboot_info* m, uint32_t w, uint32_t h);
	void glSquare(int x, int y, int w, int h, multiboot_uint32_t clr);							// Draw a square using iteration.
	void drawChar(char c, int x, int y, multiboot_uint32_t clr);								// Draw a character with an x, y, and an FG colour.
	void drawChar(char c, int x, int y, multiboot_uint32_t clr, multiboot_uint32_t bgclr);		// Draw a character with an x, y, BG colour and an FG colour.
	void glPlotPixel(int x, int y, multiboot_uint32_t clr);										// Place a pixel on the screen with an integer.
	void glPlotPixelP(int x, int y, pixel pix);													// Place a pixel on the screen with the pixel structure.

	unsigned char reverse(unsigned char b); // Find the reverse bits of a byte
};

#endif