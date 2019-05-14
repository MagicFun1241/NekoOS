#ifndef __IOPORTS_H
#define __IOPORTS_H

#include <stdint.h>
#include <string.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void outl(unsigned short port, uint32_t val);
uint32_t ind(uint16_t portid);
uint32_t inl(unsigned short port);
void outw(uint16_t port, uint16_t val);
void outd(uint16_t portid, uint32_t value);

// MEMORY MAPPED IO
uint8_t MMIORead8 (uint64_t p_address);
uint16_t MMIORead16 (uint64_t p_address);
uint32_t MMIORead32 (uint64_t p_address);
uint64_t MMIORead64 (uint64_t p_address);
void MMIOWrite8 (uint64_t p_address,uint8_t p_value);
void MMIOWrite16 (uint64_t p_address,uint16_t p_value);
void MMIOWrite32 (uint64_t p_address,uint32_t p_value);
void MMIOWrite64 (uint64_t p_address,uint64_t p_value);

#endif