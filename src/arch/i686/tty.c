#include <tty.h>
#include <stdarg.h>
#include <port.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
    {
		for (size_t x = 0; x < VGA_WIDTH; x++) 
        {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	tty_move_cursor();
}
 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) 
{
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	scroll();
	tty_move_cursor();
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void scroll()
{
    unsigned blank, temp;

    blank = 0x20 | terminal_color;

    if(terminal_row >= 25)
    {
        temp = terminal_column - 25 + 1;
        memcpy(VGA_MEMORY, VGA_MEMORY + temp * 80, (25 - temp) * 80 * 2);
        memset(VGA_MEMORY + (25 - temp) * 80, blank, 80);
        terminal_row = 25 - 1;
    }
}

void tty_move_cursor()
{
    unsigned temp = terminal_row * 80 + terminal_column;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

int printf(const char* restrict format, ...) 
{
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') 
	{
		size_t maxrem = 2147483647 - written;
 
		if(*format == '\n')
		{
			terminal_row++;
        	terminal_column=0;
			scroll();
			format++;
			continue;
		}
		else if (format[0] != '%' || format[1] == '%') 
		{
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount)
				return -1;

			terminal_write(format, amount);

			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') 
		{
			format++;
			char c = (char) va_arg(parameters, int);

			if (!maxrem)
				return -1;

			terminal_write(&c, sizeof(c));
		} 
		else if (*format == 's') 
		{
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			if (maxrem < len)
				return -1;

			terminal_write(str, len);

			written += len;
		} 
		else if (*format == 'd') 
		{
			format++;
			int num = va_arg(parameters, int);
			char* res; 
			itoa(num, res, 10);
			size_t len = strlen(res);

			if (maxrem < res)
				return -1;

			terminal_write(res, len);

			written += len;
		} 
		else 
		{
			format = format_begun_at;
			size_t len = strlen(format);

			if (maxrem < len)
				return -1;

			terminal_write(format, len);
		}
	}
 
	va_end(parameters);
	return written;
}