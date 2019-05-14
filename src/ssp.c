/*
#include <types.h>
#include <Panic.h>
#include <print.h>

// simple stack smashing protector

// 3200319470
#define STACK_CHK_GUARD 0xBEC0FFEE
 
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
 
void __stack_chk_fail(void)
{
	panic("Stack smashing detected");
}
*/