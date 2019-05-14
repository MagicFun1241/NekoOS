#include <mm.h> 
#include <string.h> 
#include <tty.h>

int has_initialized  = 0;
uint32_t last_alloc  = 0;
uint32_t heap_end    = 0;
uint32_t heap_begin  = 0;
uint32_t memory_used = 0;
int      alc_num     = 0;

#define KHEAP_MAGIC_NUMBER 0xDEADBEEF

void mm_init(uint32_t start, uint32_t end)
{
	heap_begin = start;
	heap_end   = end;
	
	memset((char *)heap_begin, 0, heap_end - heap_begin);
	
	last_alloc = heap_begin;

	printf("kheap: init kheap 0x%d - 0x%d\n", heap_begin, heap_end);
	has_initialized  = 1;
}

uint32_t mm_get_free_memory()
{
	return heap_end - heap_begin - memory_used;
}

uint32_t mm_get_allocations_count()
{
	return alc_num;
}

void *malloc(size_t size)
{
	uint32_t *current_location = 0;
	void *memory_location  = 0;
	int  alloc             = 0;
	heap_chunk_t *memory_block_hdr;
	 
	if(size <= 0)
	{  
	   return NULL;
	}
	
	if(heap_end - last_alloc < 0x520)
	{
	    last_alloc = heap_begin;	
	}

	if(heap_end - heap_begin < size)
	{
		panic("out of memory");
	}

	if(last_alloc == heap_end)
	{
	   last_alloc = heap_begin;	
	}

	current_location = (uint32_t*)heap_begin;
	
	while(heap_begin < heap_end)
	{
		memory_block_hdr = (heap_chunk_t*)current_location;

		if(memory_block_hdr->used == 0)
		{
			if(memory_block_hdr->size > size)
			{
			    memory_block_hdr->used = 1;
		        memory_location = current_location + sizeof(heap_chunk_t);
				if(memory_block_hdr->magic == KHEAP_MAGIC_NUMBER)
		        {
					alloc = 1;
					break;
				}
			}
			
			if(memory_block_hdr->size == size)
			{
			    memory_block_hdr->used = 1;
		        memory_location = current_location + sizeof(heap_chunk_t);
		        if(memory_block_hdr->magic == KHEAP_MAGIC_NUMBER)
		        {
					alloc = 1;
					break;
				}
			}
			
			if(memory_block_hdr->size == 0)
			{
			    memory_block_hdr->used  = 1;
		        memory_block_hdr->size  = size;
		        memory_block_hdr->magic = KHEAP_MAGIC_NUMBER;
				memory_location = current_location + sizeof(heap_chunk_t);
				alloc = 1;
				break;
			}
			
		if(alloc == 1)
		{
			break;
		}
		}

		if(memory_block_hdr->size != 0 && memory_block_hdr->magic == KHEAP_MAGIC_NUMBER){
			current_location += memory_block_hdr->size + sizeof(heap_chunk_t);
		} else {
			current_location++;
		}
	}
	
	if(memory_location == 0)
	{
		return NULL;
	}

	last_alloc  += size + sizeof(heap_chunk_t);
	memory_used += size;
	alc_num++;

	return memory_location;
}


void free(void *ptr) 
{
	heap_chunk_t *memory_block_hdr;

	if(ptr == 0)
	{
		return;
	}

	uintptr_t *ptr2 = (uintptr_t*)ptr - sizeof(heap_chunk_t);

	memory_block_hdr = (heap_chunk_t*)ptr2;

	if(memory_block_hdr->magic == KHEAP_MAGIC_NUMBER && memory_block_hdr->used == 1)
	{
		memory_block_hdr->used = 0;
		memory_used -= memory_block_hdr->size;
		alc_num--;
	}
	return;
}