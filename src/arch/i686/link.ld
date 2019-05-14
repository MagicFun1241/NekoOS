OUTPUT_FORMAT(elf32-i386)
ENTRY(loader)

SECTIONS
{
	begin = .; _begin = .; __begin = .;
	
  . = 0x00100000;
      .__mbHeader : {
      *(.__mbHeader)
   }
      begin = .; _begin = .; __begin = .;
   .text : { 
       *(.text) 
	}
   .rodata ALIGN (0x1000): { 
       *(.data)
	}
   .bss  : { 
	   *(COMMON)
       *(.bss)  
	}
	end = .; _end = .; __end = .;
}