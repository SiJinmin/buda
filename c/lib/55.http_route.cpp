#include "../includes/buda.h"

namespace BUDA
{
  int http_route(HttpReq* req)
  {
    goto fail;
    
    succeed: return 0;
    fail: return -1;
  }



}