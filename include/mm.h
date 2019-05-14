#ifndef DROCHY_NA_LOLI_H
#define DROCHY_NA_LOLI_H

#include <stdint.h>
#include <panic.h>

typedef struct heap_chunk_t
{
	uint8_t  used;
	size_t 	 size;
	uint32_t magic;
} __attribute__((packed)) heap_chunk_t;

void mm_init(uint32_t start, uint32_t end);
void *malloc(size_t size);
void  free(void *ptr);


#endif
