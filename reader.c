#include <fcntl.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <stdio.h>
#include <string.h>
#include <errno.h>
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
	struct timespec spec1, spec2;
	int i = 0;

	fd = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);  

	clock_gettime(CLOCK_REALTIME, &spec1);
	ret = read(fd, NULL, 0);
	clock_gettime(CLOCK_REALTIME, &spec2);

	close(fd);  

	printf("my) last: %lu : %lu from: %lu : %lu\n", spec2.tv_sec, spec2.tv_nsec, spec1.tv_sec, spec1.tv_nsec);

out:
	return ret;  
}  
