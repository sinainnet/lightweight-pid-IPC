#include <fcntl.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <stdio.h>
#include <string.h>
#include<errno.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
  
#define DEVICE_FILENAME "/dev/comchar"

int main()  
{  
	int fd;  
	int ret;
	char *p = NULL;
	char buff[64];    
	struct timespec spec1;

	fd = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);  

	printf("Writing ....\n");
	clock_gettime(CLOCK_REALTIME, &spec1);
	ret = write(fd, NULL, 0);
	if (ret < 0) {
		printf("write error!\n");
		ret = errno;
		goto out;
	}

	close(fd);  
	printf("my) %lu : %lu\n", spec1.tv_sec, spec1.tv_nsec);

out:
	return ret;  
}  
