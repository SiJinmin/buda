#include "../includes/buda.h"

namespace BUDA
{
  void show_sys_info()
  {
	  printf("void*=%ld, double=%ld, long long=%ld, long=%ld, float=%ld, int=%ld, short=%ld, u_char=%ld\n",
    sizeof(void*), sizeof(double), sizeof(long long), sizeof(long), sizeof(float), sizeof(int), sizeof(short), sizeof(u_char)); 

	  // printf("char=%d~%d, short=%d~%d, int=%d~%d, long=%ld~%ld, long long=%lld~%lld\n",
    // CHAR_MIN, CHAR_MAX, SHRT_MIN, SHRT_MAX, INT_MIN, INT_MAX, LONG_MIN, LONG_MAX, LLONG_MIN, LLONG_MAX ); 

	  printf("u_char=0~%d, u_short=0~%d, u_int=0~%u, u_long=0~%lu, u_long_long=0~%llu\n",
    UCHAR_MAX, USHRT_MAX, UINT_MAX, ULONG_MAX, ULLONG_MAX); 

  }


}