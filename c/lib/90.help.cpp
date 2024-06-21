
#include "../includes/buda.h"

namespace BUDA
{
	
void print_help(char* program_name)
{
	printf("%s -h : print help\n", program_name);
	printf("%s -m : server just prints the messages from clients.\n", program_name);
	printf("%s -w /website/root/path -p <port_number> : starts a http web server at port_number\n", program_name);
}

}