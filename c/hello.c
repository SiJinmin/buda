/* 
预处理： 
# gcc -E -o hello.i hello.c  
头文件来源： /usr/include

# gcc -S -o hello.s hello.i

# gcc -c -o hello.o hello.s

# gcc -o hello hello.o
*/

#include <stdio.h>
int main ()
{
  printf ( "hello, world \n " );
  return 0;
} 