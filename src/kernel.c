#include <kernel.h>
#include <tty.h>

void kmain()
{
	arch_init();
	printf("kernel has loaded.\n");

	for(;;);
}
  
