#include <entry.h>
#include <stdint.h>
multiboot_info_t *mboot;

multiboot_info_t *get_multiboot_table()
{
  return mboot;
}

void x86_entry(unsigned long addr, unsigned long magic)
{
  mboot = (multiboot_info_t*)addr;
  kmain();
  for(;;);
}
