#ifndef SYSTEM_H
#define SYSTEM_H

#define NULL 0

typedef unsigned char			uint8_t;
typedef unsigned short 			uint16_t;
typedef unsigned long 			uint32_t;
typedef unsigned long long 		uint64_t;
typedef signed char				int8_t;
typedef signed short 			int16_t;
typedef signed long 			int32_t;
typedef signed long long 		int64_t;
typedef unsigned int 			size_t;
typedef unsigned int 			uintptr_t;

class Ports {
public:
	static uint8_t inb(uint16_t port);
	static void outb(uint16_t port, uint8_t data);
	static uint16_t inw(uint16_t port);
	static void outw(uint16_t port, uint16_t value);
	static uint32_t inl(uint16_t port);
	static void outl(uint16_t port, uint32_t value);
};

#define inportb(port) Ports::inb(port)
#define outportb(port, data) Ports::outb(port, data)

#define inportw(port) Ports::inw(port)
#define outportw(port, data) Ports::outw(port, data)

#define inportl(port) Ports::inl(port)
#define outportl(port, data) Ports::outl(port, data)


void itoa (char *buf, int base, int d);

typedef struct tagBITMAPFILEHEADER
{
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfReserved2;
	int bOffBits;
} BITMAPFILEHEADER __attribute__((packed));

typedef struct tagBITMAPINFOHEADER
{
	int biSize;
	long biWidth;
	long biHeight;
	short biPlanes;
	short biBitCount;
	int biCompression;
	int biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
} BITMAPINFOHEADER __attribute__((packed));

typedef struct pixelsm_t {
	char r;
	char g;
	char b;
} pixelsm __attribute__((packed)); 

typedef struct pixel_t {
	char r;
	char g;
	char b;
	char a;
} pixel __attribute__((packed));

// below here is cmos rtc shit

#define CURRENT_YEAR 2018
enum {
	cmos_address 	= 0x70,
	cmos_data 		= 0x71
};

class RTC {
public:
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;

	RTC();

	int get_update_in_progress_flag();
	unsigned char get_RTC_register(int reg);

	void read_rtc();
};

#endif
