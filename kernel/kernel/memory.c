#include <kernel/memory.h>
#include <grub/multiboot.h>

#define NULL 0

MEMINFO _meminfo =
{
    .frees = 0,
    .maxFrees = 0,
    .lostSize = 0,
    .losts = 0
};

static int _memory_free(uint32_t addr, uint32_t size);

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
        /*
        printf("mmap->addr = %x | %x ,mmap->size = %x | %x ,mmap->type = %d \n",
        (uint32_t)(mmap->addr >> 32),
        (uint32_t)(mmap->addr),
        (uint32_t)(mmap->size >> 32),
        (uint32_t)(mmap->size),
        mmap->type);*/


        if (mmap->type == 2 && pre != NULL && pre->type == 1)
        {
            _memory_free(pre->addr, mmap->addr - pre->addr);
        }
        pre = mmap;
    }

    /** for debug **/
    /*
    for(uint32_t i = 0; i < _meminfo.frees; i++) {
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
