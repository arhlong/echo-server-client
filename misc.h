#ifndef _MISC_H_
#define _MISC_H_

#include "std.h"

ssize_t writen(int fd,const void *vptr,size_t n);
ssize_t readline(int fd,void *vptr,size_t len);



#endif
