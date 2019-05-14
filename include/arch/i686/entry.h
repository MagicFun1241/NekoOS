#ifndef __x86_entry_H
#define __x86_entry_H

#include <multiboot1.h>
#include <kernel.h>

void x86_entry(unsigned long addr, unsigned long magic);
multiboot_info_t *get_multiboot_table();

#endif