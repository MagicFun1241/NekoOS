# NekoOS Makefile configuration
ARCH      := i686
SRCDIRS   := src src/libc src/arch/$(ARCH)
OBJFILES  := obj/src/*.o obj/src/libc/*.o obj/src/arch/$(ARCH)/*.o
OBJDIRS	  := obj obj/src obj/src/libc obj/src/arch obj/src/arch/$(ARCH)
INCDIRS   := include/ include/libc/ include/arch/$(ARCH)
OBJDIR    := obj
ISODIR	  := iso

CROSSCOMPILER = 

# Tool Configuration
AS        := $(CROSSCOMPILER)as
ASFLAGS   :=
ASM       := nasm
ASMFLAGS  := -f elf32
CC        := $(CROSSCOMPILER)gcc
CFLAGS    := -c -ffreestanding $(foreach INCDIR, $(INCDIRS), -I$(INCDIR)) -std=c99 -Wall -nostdlib -nostartfiles #-fstack-protector-all -g -fsanitize=undefined
CXX       := $(CROSSCOMPILER)g++
CXXFLAGS  := -c -ffreestanding -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(foreach INCDIR, $(INCDIRS), -I $(INCDIR)) -std=c++0x -Wall
LD  	  := ld
LDFLAGS   := --warn-unresolved-symbols


HEADERS   :=$(foreach DIR, $(INCDIRS), $(wildcard $(DIR)/*.h))
SOURCES   :=$(foreach DIR, $(SRCDIRS), $(wildcard $(DIR)/*.asm $(DIR)/*.S $(DIR)/*.c $(DIR)/*.cpp))
OBJECTS   :=$(foreach OBJECT, $(patsubst %.asm, %.elf, $(patsubst %.S, %.O, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES))))), $(OBJDIR)/$(OBJECT))
TARGET    :=$(OBJDIR)/kernel.elf

.PHONY : all init
all : $(TARGET)
	@cd initrd; tar -cf ../iso/boot/initrd.img *
	@cp $(OBJDIR)/kernel.elf $(ISODIR)/boot
	@grub-mkrescue -o NekoOS.iso iso
	@echo You succesfully compiled NekoOS!

init :
	@mkdir $(OBJDIRS)
	@mkdir iso
	@mkdir iso/boot
	@mkdir iso/boot/grub
	@echo "insmod gfxterm;\
	insmod vbe;\
	timeout=10;\
	set gfxmode=1024x768;\
	menuentry \"NekoOS\"\
	{\
		echo 'Loading NekoOS kernel...';\
		multiboot /boot/kernel.elf;\
		echo 'Loading initrd...';\
		module    /boot/initrd.img  initrd;\
		boot;\
	}\
	" > iso/boot/grub/grub.cfg


$(TARGET) : $(OBJECTS)
	$(LD) -Tsrc/arch/$(ARCH)/link.ld   -o $@ $+


# Generic Rules

$(OBJDIR)/%.elf: %.asm
	@echo ASM $< $@
	$(ASM) $(ASMFLAGS) -o $@ $<
    	
$(OBJDIR)/%.O : %.S
	@echo AS $< $@
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o : %.c
	@echo CC $< $@
	@$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o : %.cpp
	@echo CXX $< $@
	$(CXX) $(CXXFLAGS) $< -o $@
