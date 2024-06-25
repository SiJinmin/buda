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
    log("create_mem_chain %d/%d", max_size, block_min_size);
    BudaMn(MemChain, mc); BudaMn(MemChainBlock, mcb); if(mc==NULL || mcb == NULL) return NULL;
    mcb->mem=BudaMc(block_min_size); if(mcb->mem == NULL) return NULL; mcb->size=block_min_size; 
    mc->max_size=max_size; mc->used+=mcb->size; mc->block_min_size=block_min_size; mc->first=mc->last=mcb; mc->block_count=1;
    return mc;
  }  

  MemChainBlock* mem_chain_add_block(MemChain *mc, int size)
  {
    log("mem_chain_add_block %d", size);
    if(size+mc->used > mc->max_size) { log("mem_chain reached max_size: %d", mc->max_size); return NULL; }
    BudaMn(MemChainBlock, mcb); if(mcb == NULL) return NULL;
    mcb->mem=BudaMc(size); if(mcb->mem == NULL) return NULL; mcb->size=size; mc->used+=size; mc->block_count++; mc->last->next=mcb; mc->last=mcb;
    return mcb;
  }

  void free_mem_chain(MemChain *mc)
  {
    log("free_mem_chain %d", mc->used);
    int remain_block_count = mc->block_count; MemChainBlock *mcb=mc->first, *next; BudaFree(mc);
    while(mcb) { next=mcb->next; BudaFree(mcb->mem); BudaFree(mcb); remain_block_count--; mcb=next; }
    if(remain_block_count) { log("MemChain block_count broken: %d", remain_block_count); }
  }

  void reset_mem_chain(MemChain *mc)
  {
    log("reset_mem_chain %d/%d", mc->max_size, mc->block_min_size);
    MemChainBlock *first=mc->first; MemChainBlock *mcb=first->next, *next; 
    while(mcb) { next=mcb->next; BudaFree(mcb->mem); BudaFree(mcb); mcb=next; }
    mc->block_count=1; mc->blocks_used=0; mc->last=first; mc->used=first->size;
    first->next=NULL; first->used=0;
  }

  char* use_mem_chain(MemChain *mc, int size, char* content)
  {
    log("use_mem_chain %d bytes", size);
    MemChainBlock *last=mc->last;
    if(last->used + size > last->size)
    {
      int asize=BudaMax(mc->block_min_size, size); last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL;
    }
    char* write_start=last->mem + last->used; last->used += size; mc->blocks_used+=size;
    if(content) memcpy(write_start, content, size);
    return write_start;
  }

  char* use_mem_chain(MemChain *mc, const char* format, ...)
  {
    //log("try use_mem_chain : %s", format);
    MemChainBlock *last=mc->last; int used=last->used; char *write_start=last->mem+used; int remain=last->size - used, len, asize=mc->block_min_size;
    write: va_list args; va_start(args, format); len = vsnprintf2(last->mem + used, remain, format, args); va_end(args);
    if(len == -1) 
    {
      last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL; used=0; write_start=last->mem; remain=asize; asize*=2; goto write;
    }
    else
    {
      last->used += len; mc->blocks_used+=len;
      log("use_mem_chain %d bytes : \n%s", len, write_start);
    }
    return write_start;
  }

}