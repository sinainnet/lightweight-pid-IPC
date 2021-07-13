#include <fcntl.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <stdio.h>
#include <string.h>
#include<errno.h>
  
#define DEVICE_FILENAME "/dev/comchar"

int main()  
{  
	int fd;  
	int ret;
	char *p = NULL;
	char buff[64];    

	fd = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);  

	printf("Writing ....\n");
	ret = write(fd, NULL, 0);
	if (ret < 0) {
		printf("write error!\n");
		ret = errno;
		goto out;
	}

	close(fd);  

out:
	return ret;  
}  
