#ifndef page_h
#define page_h
#define PDE_BASE 0x200000  //base addr of Page Directory
#define PTE_BASE 0x201000  //base addr of Page Table

#define ENTRY_SIZE 1024

#define PAGE_SIZE 0x1000  //size per page


/********** functions ************/
void kernel_setup_page(unsigned int totalMemory);

#endif /* page_h */
