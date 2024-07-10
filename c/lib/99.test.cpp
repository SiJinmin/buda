#include "../includes/buda.h"

namespace BUDA
{

#define LEN 100000
int num = 0;

void* thread_func(void* arg) 
{
	pthread_mutex_t* p_mutex = (pthread_mutex_t*)arg;
	for (int i = 0; i< LEN; ++i) 
	{
		pthread_mutex_lock(p_mutex);
		num += 1;
		pthread_mutex_unlock(p_mutex);
	}	
	return NULL;
}

int test_threads_mutex() 
{
	pthread_mutex_t m_mutex, *pm=&m_mutex; pthread_mutex_init(pm, NULL);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, (void*)pm);
	pthread_create(&tid2, NULL, thread_func, (void*)pm);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_mutex_destroy(pm);

	printf("correct result=%d, result=%d.\n", 2*LEN, num);
	return 0;
}



void* thread_func1(void* arg) 
{
	const char* func = "thread_func1";

	pid_t pid = getpid(); pthread_t tid = pthread_self();  
	printf("%s(%s) pid: %u, tid: %u\n", func, (char*)arg, (unsigned int)pid, (unsigned int)tid);
	
	return (void*)func;
}

void* thread_func2(void* arg) 
{
	const char* func = "thread_func2 ";

	pid_t pid = getpid(); pthread_t tid = pthread_self();
	printf("%s(%s) pid: %u, tid: %u\n", func, (char*)arg, (unsigned int)pid, (unsigned int)tid);
	
	while(1) 
	{
		printf("%s(%s) running\n", func, (char*)arg);
		sleep(1);
	}
	
	return (void*)func;
}

int test_threads()
{  
	pthread_t tid1, tid2;
	if (pthread_create(&tid2, NULL, thread_func2, (void*)"arg2")) 
	{
		printf("pthread_create error.");
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&tid1, NULL, thread_func1, (void*)"arg1")) 
	{
		printf("pthread_create error.");
		exit(EXIT_FAILURE);
	}
	
	void* rev = NULL;	pthread_join(tid1, &rev);
	printf("%s termined.\n", (char*)rev);

	pthread_cancel(tid2);		
  pthread_join(tid2, &rev);
	printf("main thread end.\n");
	return 0;
}



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

  

/*

struct timespec last_log_time = {0, 0};
regex_t regex_log_time;
if(compile_regex(Pattern_log_time, &regex_log_time)) return -1;
const char *Pattern_log_time = "\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\]\\[[0-9]+\\.[0-9]+\\]";

int check_user_input_for_log(char* input)
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
}


void read_conf()
{
	Link *mem=link_create();

  char* content=NULL; int len; if((len=file_content_get((char*)Conf_path, NULL, mem, &content)) > 0) 
	{
		char *pc=content, *pc_end=pc + len, c;
		while(pc < pc_end)
		{
			c=*pc; if(c=='#' || c=='\r' || c=='\n') {pc=next_line(pc); if(pc==NULL) break; else continue;}
			if(strncmp(pc, "pw=", 3)==0 && pc+3<pc_end)
			{
				admin_pw=pc+3; pc=set_next_space_0(pc, pc_end);
			} else pc++;
		}
		// printf("admin_pw: %s\n", admin_pw);
	}

	BudaF(mem);
}


*/

}