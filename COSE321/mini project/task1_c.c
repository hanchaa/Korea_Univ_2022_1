#include<stdio.h>

void selection_sort(int *d);

int task1_c()
{
	unsigned int delay_count =  0x1000000;
	unsigned char *LED=(unsigned char *) 0x41200000;
	int result[10];
	int data[] = {8,2,4,3,6,7,1,5,10,9};
	unsigned int i,k;
	k=0;

	while(1){

		for(i=0; i<10 ;i++) result[i] = data[i];

		selection_sort(result);          // sorting data

		for(i = 0; i<delay_count; i++);	// time delay to see LED outputs

		*LED=(unsigned char)result[k++];

		if(k == 10) k = 0;
	}

	return 0;
}

void selection_sort(int *d)
{
  int i,j,t,min;

  for(i=0; i<9; i++)
  {
	min = i;
	for(j=i+1; j<10; j++){
		if (d[min]>d[j])
			min = j;
	}
	t=d[i];
	d[i]=d[min];
	d[min]=t;
  }

  return;
}
