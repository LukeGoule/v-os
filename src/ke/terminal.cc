#include <terminal.h>

// Do not call this!!!
Terminal::Terminal() {

}

Terminal::Terminal(VGA v) {
	this->v = v;
}

void Terminal::Print(char* str) {
	char c;
	while ((c = *str++) != '\0') {
		this->PrintChar(c);
	}
}

void Terminal::PrintChar(char c) {
	switch (c) {
	case '\n':
		this->cursorY += 16;
		this->cursorX = 0;
		break;
	case '\t':
		this->cursorX += 8+1;
		break;
	case '\b':
		if (this->cursorX > 0) {
			this->cursorX -= 8+1;
			this->v.glSquare(this->cursorX, this->cursorY, 9, 16, this->bgcolour);
		}
		break;
	default:
		this->v.drawChar(c, this->cursorX, this->cursorY, this->colour, this->bgcolour);
		this->cursorX += (8)+1;
		break;
	}

	if (this->cursorX > this->v.w - 16) {
		this->cursorY += 16;
		this->cursorX = 0;
	}

	if (this->cursorY > this->v.h - 8) {
	 	cursorY = 0;

	 	for (int x = 0; x < 1024; x++) {
	 		for (int y = 0; y < 768; y++) {
	 			multiboot_uint32_t *pix = v.fb + v.mbi->framebuffer_pitch * y + 4 * x;
	 			*pix = 0;
	 		}
	 	}
	}
}

void Terminal::printf(const char* format, ...) {
	char **arg = (char**) &format;
	int c;
	char buf[20];

	arg++;

	while ((c = *format++) != '\0') {
		char* p;

		if (c != '%') {
			this->PrintChar(c);
		} else {
			c = *format++;
			switch (c) {
			case 'd':
			case 'u':
			case 'x':
				itoa(buf, c, *((int*) arg++));
				this->Print(buf);
				break;
			case 's':
				this->Print(*((int*) arg++));
				break;
			case 'f':
				double f = *((double*)arg++);
				int a = (int)f;

				this->printf("%d.", a);

				if(f<0)                           /*  If negative, remove the sign  */
				   f = -f;
				f = (f - (int)f) * 1000000;   /*  Get numbers after decimal point  */
				
				this->printf("%d", (int)f);
				
				break;
			default:
				this->PrintChar(*((int*) arg++));
				break;
			}
		}
	}
}

void Terminal::clear() {
	this->v.glSquare(0,0, this->v.w, this->v.h, this->bgcolour); // cheap method to remove everything lol
}