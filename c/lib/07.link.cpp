#include "../includes/buda.h"

namespace BUDA
{
  Link* create_link()
  {
    // log("create_link %d/%d", max_size, item_min_size);
    BudaMn(Link, link); if(link==NULL) return NULL;  
    return link;
  }  

  LinkItem* link_append_item(Link *link, void* content)
  {
    // log("link_append_item: %d", size);
    BudaMn(LinkItem, item); if(item == NULL) return NULL; item->content=content;
    LinkItem* last = link->last;
    if(last == NULL){ link->first=link->last=item; }
    else{ item->pre=last; last->next=item; link->last=item;  }    
    link->item_count++; 
    return item;
  }

  int concat_link(Link *link, Link *link2)
  {
    //log("concat_link %d:%d", link->content_used, link2->content_used);
    if(link2==NULL) return link->item_count; 
    LinkItem *last2=link2->last;
    if(last2) 
    {
      LinkItem* last = link->last;
      if(last == NULL){ link->first=link2->first;  }
      else { LinkItem *first2=link2->first; last->next = first2; first2->pre=last; } 
      link->last=last2; link->item_count+=link2->item_count;
    }
    BudaFree(link2);
    return link->item_count;
  }

  void free_link(Link *link, free_obj free_item_content)
  {
    //log("free_link %d", link->used);
    int remain_item_count = link->item_count; LinkItem *item=link->first, *next; BudaFree(link);
    while(item) { next=item->next; free_item_content(item->content); BudaFree(item); remain_item_count--; item=next; }
    if(remain_item_count) { log("Link item_count broken: %d", remain_item_count); }
  }

  void reset_link(Link *link)
  {
    // log("reset_link %d/%d", link->max_size, link->item_min_size);
    LinkItem *first=link->first; LinkItem *item=first, *next; 
    while(item) { next=item->next; BudaFree(item); item=next; }
    link->first = link->last=0; link->item_count=0; 
  }

}