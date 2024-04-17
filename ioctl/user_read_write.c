#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#define RD_VALUE _IOR('a','b',int32_t *)
#define WR_VALUE _IOW('a','b',int32_t *)
//#define WR_VALUE _IOW('a','b',int32_t *)
//#define RD_VALUE _IOR('a','b',int32_t *)

int32_t value,number;
int main()
{
	int option,fd = open("/dev/Dinesh_Dev",O_RDWR);
	if(fd < 0)
	{
		printf("Cannot be open device file...\n");
		return 0;
	}
	while(1)
	{
		printf("Please choose the option...\n1.Write\n2.Read\n3.Exit\n");
		scanf("%d",&option);
		switch(option)
		{
			case 1: printf("Enter the value for ioctl\n");
				scanf(" %d",&number);
				ioctl(fd,WR_VALUE,(int32_t *) &number);
				printf("Writing Successfully\n");
				break;
			case 2: ioctl(fd,RD_VALUE,(int32_t *) &value);
				printf("Reading data = %d\n",value);
				break;
			case 3:printf("Exited......\n");
			       return 0;
		}
	}
}
