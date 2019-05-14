#include <port.h>

void outb(uint16_t port, uint8_t val)
{
    __asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
     __asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
} 

uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outl(unsigned short port, uint32_t val)
{
    __asm volatile( "outl %0, %1"
            : : "a"(val), "Nd"(port) );
}
uint32_t ind(uint16_t portid)
{
	uint32_t ret;
	__asm__("inl %%dx, %%eax":"=a"(ret) : "d"(portid));
	return ret;
}

uint32_t inl(unsigned short port)
{
    uint32_t ret;
    __asm volatile( "inl %1, %0"
            : "=a"(ret) : "Nd"(port) );
    return ret;
}
void outw(uint16_t port, uint16_t val)
{
	__asm__("outw %%ax, %%dx": : "d" (port), "a" (val));
}

void outd(uint16_t portid, uint32_t value)
{
	__asm__("outl %%eax, %%dx": : "d" (portid), "a" (value));
}

// MEMORY MAPPED IO
uint8_t MMIORead8 (uint64_t p_address)
{
    return *((volatile uint8_t*)(uintptr_t)(p_address));
}
uint16_t MMIORead16 (uint64_t p_address)
{
    return *((volatile uint16_t*)(uintptr_t)(p_address));
}
uint32_t MMIORead32 (uint64_t p_address)
{
    return *((volatile uint32_t*)(uintptr_t)(p_address));
}
uint64_t MMIORead64 (uint64_t p_address)
{
    return *((volatile uint64_t*)(uintptr_t)(p_address));
}
void MMIOWrite8 (uint64_t p_address,uint8_t p_value)
{
    (*((volatile uint8_t*)(uintptr_t)(p_address)))=(p_value);
}
void MMIOWrite16 (uint64_t p_address,uint16_t p_value)
{
    (*((volatile uint16_t*)(uintptr_t)(p_address)))=(p_value);
}
void MMIOWrite32 (uint64_t p_address,uint32_t p_value)
{
    (*((volatile uint32_t*)(uintptr_t)(p_address)))=(p_value);
}
void MMIOWrite64 (uint64_t p_address,uint64_t p_value)
{
    (*((volatile uint64_t*)(uintptr_t)(p_address)))=(p_value);
}
