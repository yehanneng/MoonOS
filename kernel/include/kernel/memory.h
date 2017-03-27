#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <grub/multiboot.h>

#define MEMORY_FREES 1000
typedef struct _freeinfo{
	uint32_t addr;
	uint32_t size;
} FREEINFO;

typedef struct _memoryinfo{
	int frees;      //avalible free count
    int maxFrees;   //max avalible free count
    int lostSize;   // total free failed size
    int losts;      // free failed count
	FREEINFO free[MEMORY_FREES];
} MEMINFO;

#ifdef __cplusplus
extern "C" {
#endif

/* inplement in memory.c */
uint32_t parse_memory_info(multiboot_info_t *mbf);

unsigned int memory_alloc_4k(unsigned int size);

int mempry_free_4k(unsigned int addr, unsigned int size);

void *kalloc_page(int pages);

int kfree_page(void *ptr, int pages);

#ifdef __cplusplus
}
#endif

#endif // MEMORY_H
