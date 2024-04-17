#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>

int8_t read_buf[1024];
int8_t write_buf[1204];
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
			case 1: printf("Enter the any stringfor writing the kernel space\n");
				scanf(" %[^\n]",write_buf);
				write(fd,write_buf,strlen(write_buf)+1);
				printf("Writing Successfully\n");
				break;
			case 2: read(fd,read_buf,1024);
				printf("Reading data = %s\n",read_buf);
				break;
			case 3:printf("Exited......\n");
			       return 0;
		}
	}
}
