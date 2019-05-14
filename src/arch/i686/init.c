#include <init.h>
#include <stdint.h>

int arch_init()
{
	terminal_initialize();

	multiboot_info_t *mboot = get_multiboot_table();
	uint32_t initrd_beg = *(uint32_t*)(mboot->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(mboot->mods_addr+4);

	
	mm_init(initrd_end, initrd_end + 0x400000);

	gdt_install();
    idt_install();

	printf("NekoOS kernel has loaded.");
	return 1;

	
}

