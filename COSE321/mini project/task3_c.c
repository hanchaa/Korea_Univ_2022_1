#include <stdio.h>

int task3_c()
{
	unsigned int delay_count = 0x1000000;
	unsigned int i, cnt;

	cnt = 1;

   while(1) {

 		for(i = 0; i<delay_count; i++);	//delay

		printf("\n<<< Hello from Task3! %d times >>> \n", cnt++);

   }

  return 0;
  
}
