#include <kernel/memory.h>
#include <grub/multiboot.h>
#include <kernel/page.h>

#define NULL 0

MEMINFO _meminfo =
{
    .frees = 0,
    .maxFrees = 0,
    .lostSize = 0,
    .losts = 0
};

static int _memory_free(uint32_t addr, uint32_t size);
static void _format_meminfo();

void parse_memory_info(multiboot_info_t* memoryInfo)
{
    uint32_t total_memory = 0; //byte
    uint32_t memory_slots = memoryInfo->mmap_length / sizeof(multiboot_memory_map_t);
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) memoryInfo->mmap_addr;
    multiboot_memory_map_t* pre = NULL;
    for (uint32_t i = 0; i < memory_slots; i++, mmap++)
    {
        if(total_memory < ((uint32_t)mmap->addr + (uint32_t)mmap->len))
        {
            total_memory = ((uint32_t)mmap->addr +(uint32_t)mmap->len);
        }

        if (mmap->type == 2 && pre != NULL && pre->type == 1)
        {
            _memory_free(pre->addr, mmap->addr - pre->addr);
        }
        pre = mmap;
    }

    _format_meminfo();

    /** for debug **/
    /*
    for(int i = 0; i < _meminfo.frees; i++)
    {
        printf("freeInfo.addr = %x | freeInfo.size = %x \n", _meminfo.free[i].addr, _meminfo.free[i].size);
    }
    */

}

static int _memory_free(uint32_t addr, uint32_t size)
{
    int i;
    for (i = 0; i < _meminfo.frees; i++)
    {
        if (_meminfo.free[i].addr > addr)
        {
            break;
        }
    }

    if (i > 0)    //check if can link to the pre free memory
    {
        if (_meminfo.free[i - 1].addr + _meminfo.free[i - 1].size == addr)
        {
            //can link to pre free memory
            _meminfo.free[i - 1].size += size;

            if (i < _meminfo.frees)   //check if can link next memory
            {
                if (addr + size == _meminfo.free[i].addr)
                {
                    _meminfo.free[i - 1].size += _meminfo.free[i].size;
                    _meminfo.frees--;
                    for (; i < _meminfo.frees; i++)
                    {
                        _meminfo.free[i] = _meminfo.free[i + 1];
                    }
                }
            }
            return 0;
        }
    }

    if (i < _meminfo.frees)
    {
        if (addr + size == _meminfo.free[i].addr)
        {
            _meminfo.free[i].addr = addr;
            _meminfo.free[i].size += size;
            return 0;
        }
    }

    /* single free memory */
    //need to creat a new free memory
    if (_meminfo.frees < MEMORY_FREES)
    {
        int j;
        for (j = _meminfo.frees; j > i; j--)
        {
            //_meminfo.free[j] = _meminfo.free[j - 1];
            _meminfo.free[j].addr = _meminfo.free[j - 1].addr;
            _meminfo.free[j].size = _meminfo.free[j - 1].size;
        }

        _meminfo.frees++;
        _meminfo.free[i].addr = addr;
        _meminfo.free[i].size = size;
        return 0;
    }

    // failed
    _meminfo.losts++;
    _meminfo.lostSize += size;
    return -1;
}

static void _format_meminfo()
{
    // mark video memory in memory info
    FREEINFO* info = &_meminfo.free[0];
    info->size = (0xB8000 - info->addr);


    memory_alloc_4k(0x1000);
}

static uint32_t memory_alloc(uint32_t size)
{
    uint32_t result = 0;
    int i;
    for (i = 0; i < _meminfo.frees; i++)
    {
        if (_meminfo.free[i].size >= size)    //find free memory
        {
            result = _meminfo.free[i].addr;
            _meminfo.free[i].addr += size;
            _meminfo.free[i].size -= size;

            if (_meminfo.free[i].size == 0)
            {
                _meminfo.frees--;
                for (; i < _meminfo.frees; i++)
                {
                    _meminfo.free[i] = _meminfo.free[i + 1];
                }
            }
            return result;
        }
    }
    return 0;
}

static int memory_free(uint32_t addr, uint32_t size)
{
    int i;
    for (i = 0; i < _meminfo.frees; i++)
    {
        if (_meminfo.free[i].addr > addr)
        {
            break;
        }
    }

    if (i > 0)    //check if can link to the pre free memory
    {
        if (_meminfo.free[i - 1].addr + _meminfo.free[i - 1].size == addr)
        {
            //can link to pre free memory
            _meminfo.free[i - 1].size += size;

            if (i < _meminfo.frees)   //check if can link next memory
            {
                if (addr + size == _meminfo.free[i].addr)
                {
                    _meminfo.free[i - 1].size += _meminfo.free[i].size;
                    _meminfo.frees--;
                    for (; i < _meminfo.frees; i++)
                    {
                        _meminfo.free[i] = _meminfo.free[i + 1];
                    }
                }
            }
            return 0;
        }
    }

    if (i < _meminfo.frees)
    {
        if (addr + size == _meminfo.free[i].addr)
        {
            _meminfo.free[i].addr = addr;
            _meminfo.free[i].size += size;
            return 0;
        }
    }

    /* single free memory */
    //need to creat a new free memory
    if (_meminfo.frees < MEMORY_FREES)
    {
        int j;
        for (j = _meminfo.frees; j > i; j--)
        {
            _meminfo.free[j] = _meminfo.free[j - 1];
        }

        _meminfo.frees++;
        _meminfo.free[i].addr = addr;
        _meminfo.free[i].size = size;
        return 0;
    }

    // failed
    _meminfo.losts++;
    _meminfo.lostSize += size;
    return -1;
}

uint32_t memory_alloc_4k(uint32_t size)
{
    uint32_t a;
	size = (size + 0xfff) & 0xfffff000;
	a = memory_alloc(size);
	return a;
}

int mempry_free_4k(uint32_t addr, uint32_t size)
{
	int i;
	size = (size + 0xfff) & 0xfffff000;
	i = memory_free(addr, size);
	return i;
}

void* kalloc_page(int pages)
{
    return (void*)memory_alloc_4k(pages * PAGE_SIZE);
}

int kfree_page(void* ptr, int pages)
{
    return mempry_free_4k((uint32_t)ptr, pages * PAGE_SIZE);
}
