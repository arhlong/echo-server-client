#include "misc.h"

ssize_t writen(int fd,const void *vptr,size_t n)
{
	ssize_t nwrite;
	size_t nleft;
	const char *ptr = (char *)vptr;
	nleft = n;

	while(nleft > 0)
	{
		nwrite = write(fd,ptr,nleft);
		if(nwrite > 0)
		{
			ptr += nwrite;
			nleft -= nwrite;
		}
		else if(nwrite < 0 && errno == EINTR)
		{
			nwrite = 0;
			perror("write");
			continue;
		}
		else if(nwrite < 0)
		{
			perror("write");
			return -1;
		}
	}

	return n;
}


ssize_t readline(int fd,void *vptr,size_t len)
{
	char tmp;
	char *ptr = (char *)vptr;
	int n,rc;

	for(n = 1;n < len;n ++)
	{
		rc = read(fd,&tmp,1);
		if(rc > 0)
		{
			*ptr++ = tmp;
			if(tmp == '\n')
			{
				break; 
			}
		}
		else if(rc == 0)
		{
			*ptr = 0;
			return (n - 1);
		}
		else if(rc < 0 && errno == EINTR)
		{
			n --;
			perror("read");
			continue;
		}
		else if(rc < 0)
		{
			perror("read");
			return -1;
		}
	}

	*ptr = 0;
	return n;
}
