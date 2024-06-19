

#include "../includes/buda.h"


struct tm *localtime_s(struct tm *result, time_t *timep)
{
	return localtime_r(timep, result);
}

struct tm * gmtime_s(struct tm *result, time_t *timep)
{
		return gmtime_r(timep, result);
}

