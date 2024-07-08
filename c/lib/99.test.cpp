#include "../includes/buda.h"

namespace BUDA
{
  void test_print_buf()
  {    
	  char buf[10] , buf_time[10]; buf[9]='a', buf_time[9]='a'; int len; tm t, *p=&t;
    //len =	strftime(buf_time, 10, "123456789", p); printf("len = %d, buf_time=%s\n", len, buf_time); 
    //len =	snprintf(buf, 10, "1234567890"); printf("len = %d, buf=%s\n", len, buf);     
    //len =	snprintf(buf, 10, "12345678901"); printf("len = %d, buf=%s\n", len, buf);
  }

  void test_bit_shift()
  {
	  char a=0b10000000; u_char b=0b10000000; a=(a>>4); b=(b>>4); printf("a=%d, b=%d\n", (int)a, (int)b);
  }

  
	//if(compile_regex(Pattern_log_time, &regex_log_time)) return -1;


/*int check_user_input_for_log(char* input)
{
  int r = search_first(input, &regex_log_time); if(r>=1) 
	{ 
		char filename[PATH_MAX]; int len; if((len=snprintf2(filename, PATH_MAX_1, "%s", log_input_dir))>0)
		{  
		  if(time_text(filename+len, PATH_MAX_1-len, 'f')>0) 
			{ file_write(filename, input, strlen(input)); log("input from user contains forbidden info: %s", filename);  }
		}
    return -1;
	}
  return 0;
}*/

}