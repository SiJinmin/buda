#include "../includes/buda.h"

void print_help(char* program_name)
{
	printf("%s -h : print help\n", program_name);
	printf("%s -m : server just prints the messages from clients.\n", program_name);
	printf("%s -w /website/root/path : starts a http web server\n", program_name);
}