#include "../includes/buda.h"

namespace BUDA
{
  char* calloc2(int size, Link *mem)
  {
    if (size <= 0) { log("alloc size cannot be less than 1: %d", size); return NULL; }
    void* r = calloc(1, size); if (!r) { log("alloc %d bytes memory failed.", size); return NULL; }  
    if(mem) link_append_item(mem, r);
    return (char*)r;
  }

  MemChain* mem_chain_create(int max_size, int block_min_size)
  {
    // log("mem_chain_create %d/%d", max_size, block_min_size);
    BudaMn(MemChain, mc); BudaMn(MemChainBlock, mcb); if(mc==NULL || mcb == NULL) return NULL;
    mcb->mem=BudaMc(block_min_size); if(mcb->mem == NULL) return NULL; mcb->size=block_min_size; 
    mc->max_size=max_size; mc->used+=mcb->size; mc->block_min_size=block_min_size; mc->first=mc->last=mcb; mc->block_count=1;
    return mc;
  }  

  MemChainBlock* mem_chain_add_block(MemChain *mc, int size)
  {
    log("mem_chain_add_block: %d", size);
    if(size+mc->used > mc->max_size) { log("mem_chain reached max_size: %d", mc->max_size); return NULL; }
    BudaMn(MemChainBlock, mcb); if(mcb == NULL) return NULL;  mcb->mem=BudaMc(size); if(mcb->mem == NULL) return NULL; 
    mcb->size=size; mc->used+=size; mc->block_count++; mc->last->next=mcb; mc->last=mcb;
    return mcb;
  }

  int mem_chain_concat(MemChain *mc, MemChain *mc2)
  {
    //log("mem_chain_concat %d:%d", mc->content_used, mc2->content_used);
    mc->last->next = mc2->first; mc->last=mc2->last; mc->block_count+=mc2->block_count; mc->content_used+=mc2->content_used; mc->used+=mc2->used;
    BudaFree(mc2);
    return mc->content_used;
  }

  void mem_chain_free(MemChain *mc)
  {
    //log("mem_chain_free %d", mc->used);
    int remain_block_count = mc->block_count; MemChainBlock *mcb=mc->first, *next; BudaFree(mc);
    while(mcb) { next=mcb->next; BudaFree(mcb->mem); BudaFree(mcb); remain_block_count--; mcb=next; }
    if(remain_block_count) { log("MemChain block_count broken: %d", remain_block_count); }
  }

  void mem_chain_reset(MemChain *mc)
  {
    // log("mem_chain_reset %d/%d", mc->max_size, mc->block_min_size);
    MemChainBlock *first=mc->first; MemChainBlock *mcb=first->next, *next; 
    while(mcb) { next=mcb->next; BudaFree(mcb->mem); BudaFree(mcb); mcb=next; }
    mc->block_count=1; mc->content_used=0; mc->last=first; mc->used=first->size;
    first->next=NULL; first->used=0;
  }

  char* mem_chain_use(MemChain *mc, int size, char* content)
  {
    // log("mem_chain_use %d bytes", size);
    MemChainBlock *last=mc->last;
    if(last->used + size > last->size)
    {
      int asize=BudaMax(mc->block_min_size, size); last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL;
    }
    char* write_start=last->mem + last->used; last->used += size; mc->content_used+=size;
    if(content) memcpy(write_start, content, size);
    return write_start;
  }

  char* mem_chain_use(MemChain *mc, const char* format, ...)
  {
    //log("try mem_chain_use : %s", format);
    MemChainBlock *last=mc->last; int used=last->used; char *write_start=last->mem+used; int remain=last->size - used, len, asize=mc->block_min_size;
    write: va_list args; va_start(args, format); len = vsnprintf2(last->mem + used, remain, format, args); va_end(args);
    if(len == -1) 
    {
      last = mem_chain_add_block(mc, asize); if(last==NULL) return NULL; used=0; write_start=last->mem; remain=asize; asize*=2; goto write;
    }
    else
    {
      last->used += len; mc->content_used+=len;
      // log("mem_chain_use %d bytes : \n%s", len, write_start);
    }
    return write_start;
  }

}