#include "liballoc.h"
#include <kernel/memory.h>

int liballoc_lock()
{
	asm("cli");
	return 0;
}


int liballoc_unlock()
{
	asm("sti");
	return 0;
}



void* liballoc_alloc(size_t pages)
{
	void* a = kalloc_page(pages);
	return a;
}

int liballoc_free(void* ptr, size_t pages)
{
	return kfree_page(ptr, pages);
}
