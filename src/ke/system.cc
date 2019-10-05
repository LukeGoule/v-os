#include <system.h>

static uint8_t Ports::inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
static void Ports::outb(uint16_t port, uint8_t data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

static uint16_t Ports::inw(uint16_t port) {
	uint16_t ret;
	asm volatile("inw %%dx, %%ax":"=a"(ret):"d"(port));
	return ret;
}
static void Ports::outw(uint16_t port, uint16_t value) {
	asm volatile("outw %%ax, %%dx": :"d" (port), "a" (value));
}

static uint32_t Ports::inl(uint16_t port) {
	uint32_t ret;
	asm volatile("inl %%dx, %%ax":"=a"(ret):"d"(port));
	return ret;
}
static void Ports::outl(uint16_t port, uint32_t value) {
	asm volatile("outl %%ax, %%dx": :"d" (port), "a" (value));
}
/* Convert the integer D to a string and save the string in BUF. If
BASE is equal to ’d’, interpret that D is decimal, and if BASE is
equal to ’x’, interpret that D is hexadecimal. */
void itoa (char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	/* If %d is specified and D is minus, put ‘-’ in the head. */
	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
		divisor = 16;
	/* Divide UD by DIVISOR until UD == 0. */
	do
	{
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	}
	while (ud /= divisor);
	/* Terminate BUF. */
	*p = 0;
	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

RTC::RTC() {
	
}

int RTC::get_update_in_progress_flag() {
	Ports::outb(cmos_address, 0x0A);
	return (Ports::inb(cmos_data) & 0x80);
}

unsigned char RTC::get_RTC_register(int reg) {
	Ports::outb(cmos_address, reg);
	return (Ports::inb(cmos_data));
}

int century_register = 0x00;

void RTC::read_rtc() {
	unsigned char century;
	unsigned char last_second;
	unsigned char last_minute;
	unsigned char last_hour;
	unsigned char last_day;
	unsigned char last_month;
	unsigned char last_year;
	unsigned char last_century;
	unsigned char registerB;

	while (this->get_update_in_progress_flag());
	second = get_RTC_register(0x00);
	minute = get_RTC_register(0x02);
	hour = get_RTC_register(0x04);
	day = get_RTC_register(0x07);
	month = get_RTC_register(0x08);
	year = get_RTC_register(0x09);
	if (century_register != 0) {
		century =get_RTC_register(century_register);
	}

	do {
		last_second = second;
		last_minute = minute;
		last_hour = hour;
		last_day = day;
		last_month = month;
		last_year = year;
		last_century = century;

		while (get_update_in_progress_flag());
		second = get_RTC_register(0x00);
		minute = get_RTC_register(0x02);
		hour = get_RTC_register(0x04);
		day = get_RTC_register(0x07);
		month = get_RTC_register(0x08);
		year = get_RTC_register(0x09);
		if (century_register != 0) {
			century =get_RTC_register(century_register);
		}
	} while (last_second != second || last_minute != minute || last_hour != hour || last_day != day || last_month != month || last_year != year || last_century != century);

	registerB = get_RTC_register(0x0B);

	if (!(registerB & 0x04)) {
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = (hour & 0x0F) + (((hour & 0x70) / 16) * 10) | (hour & 0x80);
		day = (day & 0x0F) + ((day / 16) * 10);
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
		if (century_register != 0) {
			century = (century & 0x0F) + ((century / 16) * 10);
		}
	}

	if (!(registerB & 0x02) && (hour & 0x20)) {
		hour = ((hour & 0x7F) + 12) % 24;
	}

	if (century_register != 0) {
		year += century;
	} else {
		year += (CURRENT_YEAR / 100) * 100;
		if (year < CURRENT_YEAR) year += 100;
	}
}