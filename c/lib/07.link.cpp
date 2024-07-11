#include "../includes/buda.h"

namespace BUDA
{
  Link* link_create(Link *mem)
  {
    // log("link_create %d/%d", max_size, item_min_size);
    BudaMnl(Link, link, mem); if(link==NULL) return NULL;  
    if(mem) link->mem=mem;
    return link;
  }  

  LinkItem* link_append_item(Link *link, void* content)
  {
    // log("link_append_item: %d", size);
    Link *mem=link->mem; BudaMnl(LinkItem, item, mem); if(item == NULL) return NULL; item->content=content;
    LinkItem* last = link->last;
    if(last == NULL){ link->first=link->last=item; }
    else{ item->pre=last; last->next=item; link->last=item;  }    
    link->item_count++; 
    return item;
  }

  void link_remove_first(Link *link)
  {
    LinkItem* first = link->first; if(first == NULL) return; 
    LinkItem* second = first->next; if(second){ second->pre=NULL; } 
    link->first=second; link->item_count--; 
    if(link->mem==NULL) free(first);
  }

  int link_concat(Link *link, Link *link2)
  {
    //log("link_concat %d:%d", link->content_used, link2->content_used);
    if(link2==NULL) return link->item_count; 
    Link *mem=link->mem; LinkItem *last2=link2->last;
    if(last2) 
    {
      LinkItem* last = link->last;
      if(last == NULL){ link->first=link2->first;  }
      else { LinkItem *first2=link2->first; last->next = first2; first2->pre=last; } 
      link->last=last2; link->item_count+=link2->item_count;
    }
    if(mem==NULL) BudaFree(link2);
    return link->item_count;
  }

  void link_free(Link *link)
  {
    Link *mem=link->mem; if(mem) return;
    //log("link_free %d", link->used);
    int remain_item_count = link->item_count; LinkItem *item=link->first, *next; BudaFree(link);
    while(item) { next=item->next; BudaFree(item); remain_item_count--; item=next; }
    if(remain_item_count) { log("Link item_count broken: %d", remain_item_count); }
  }

  void link_reset(Link *link)
  {
    // log("link_reset %d/%d", link->max_size, link->item_min_size);
    Link *mem=link->mem; if(mem==NULL)
    {
      LinkItem *first=link->first; LinkItem *item=first, *next; 
      while(item) { next=item->next; BudaFree(item); item=next; }
    }
    link->first = link->last=0; link->item_count=0; 
  }

  MemObj* mem_obj_create()
  {
    BudaMn(Link, mem); if(mem==NULL) return NULL;
    BudaMn(MemObj, mo); if(mo==NULL) return NULL; mo->mem=mem;   
    link_append_item(mem, mo);
    return mo;
  }

}