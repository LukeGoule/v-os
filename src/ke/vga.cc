#include <vga.h>
#include <font.h>

VGA::VGA(multiboot_info* m, uint32_t w, uint32_t h) {
	this->mbi = m;
	this->fb = (void *) (unsigned long) mbi->framebuffer_addr;

	this->w = w;
	this->h = h;
}

void VGA::glPlotPixel(int x, int y, multiboot_uint32_t clr) {
	multiboot_uint32_t *pix = fb + mbi->framebuffer_pitch * y + 4 * x;
	clr = clr | 0xFF000000;
 	*pix = clr;
}

void VGA::glPlotPixelP(int x, int y, pixel pix) {
	pixel *p = (pixel*)(fb + mbi->framebuffer_pitch * y + 4 * x);
	pix.a = 0xFF;
	*p = pix;
}

void VGA::glSquare(int x, int y, int w, int h, multiboot_uint32_t clr) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			this->glPlotPixel(x+i,y+j,clr);
		}
	}
}

unsigned char VGA::reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void VGA::drawChar(char c, int x, int y, multiboot_uint32_t clr) {
	drawChar(c, x, y, clr, 0x0); // Draw a char with black as the background default
}

void VGA::drawChar(char c, int x, int y, multiboot_uint32_t clr, multiboot_uint32_t bgclr) {
	for (int j = 0; j < 16; j++) {
		for (int i = 0; i < 8; i++) {
			if (bit(this->reverse(VidFontIso8x16Data[((c - ' ')*16)+j]), i)==1) {
				this->glSquare(x + (i), y + (j), 1,1, clr);
			} else {
				this->glSquare(x + (i), y + (j), 1,1, bgclr);
			}
		}
	}
}