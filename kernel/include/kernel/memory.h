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


/* inplement in memory.c */
void parse_memory_info(multiboot_info_t* mbf);

#endif // MEMORY_H
