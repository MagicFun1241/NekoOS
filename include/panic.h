#ifndef PANIC_H
#define PANIC_H

#define panic(x) _panic(x, __FUNCTION__, __FILE__, __LINE__)

void _panic(char* reason, char const *function, char const *file, unsigned long line);

#endif
