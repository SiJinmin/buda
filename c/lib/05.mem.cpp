#include "../includes/buda.h"

namespace BUDA
{
  void* alloc(int size)
  {
    if (size <= 0) { log("alloc size cannot be less than 1\n"); return NULL; }
    void* r = calloc(1, size); if (!r) { log("alloc %d bytes memory failed.\n", size); return NULL; }  
    return r;
  }

  MemChain* create_mem_chain(int max_size, int block_min_size)
  {
    BudaMn(MemChain, mc); BudaMn(MemChainBlock, mcb); if(mc==NULL || mcb == NULL) return NULL;
    mcb->mem=BudaMc(block_min_size); if(mcb->mem == NULL) return NULL; mcb->size=block_min_size; 
    mc->max_size=max_size; mc->used+=mcb->size; mc->block_min_size=block_min_size; mc->first=mc->last=mcb; mc->block_count=1;
    return mc;
  }  

  MemChainBlock* mem_chain_add_block(MemChain *mc, int size)
  {
    if(size+mc->used > mc->max_size) { log("mem_chain reached max_size: %d", mc->max_size); return NULL; }
    BudaMn(MemChainBlock, mcb); if(mcb == NULL) return NULL;
    mcb->mem=BudaMc(size); if(mcb->mem == NULL) return NULL; mcb->size=size; mc->used+=size; mc->block_count++; mc->last->next=mcb; mc->last=mcb;
    return mcb;
  }

  void free_mem_chain(MemChain *mc)
  {
    int remain_block_count = mc->block_count; MemChainBlock *mcb=mc->first, *next; BudaFree(mc);
    while(mcb) { next=mcb->next; BudaFree(mcb->mem); BudaFree(mcb); remain_block_count--; mcb=next; }
    if(remain_block_count) { log("MemChain block_count broken: %d", remain_block_count); }
  }

  char* use_mem_chain(MemChain *mc, int size)
  {
    MemChainBlock *last=mc->last;
    if(last->used + size > last->size)
    {
      int asize=BudaMax(mc->block_min_size, size); last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL;
    }
    int write_start=last->used; last->used += size; mc->blocks_used+=size;
    return last->mem + write_start;
  }

  char* use_mem_chain(MemChain *mc, char* format, ...)
  {
    MemChainBlock *last=mc->last; int used=last->used, write_start=used, remain=last->size - used, len, asize=mc->block_min_size;
    write: va_list args; va_start(args, format); len = vsnprintf(last->mem + used, remain, format, args); va_end(args);
    if(len<0 || len>=remain) 
    {
      last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL; used=write_start=0; remain=asize; asize*=2; goto write;
    }
    else
    {
      last->used += len; mc->blocks_used+=len;
    }
    return last->mem + write_start;
  }

}