#include "npheap.h"
#include <npheap/npheap.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

void *npheap_alloc(int devfd, __u64 offset, __u64 size)
{
     __u64 aligned_size= ((size + getpagesize() - 1) / getpagesize())*getpagesize();
     return mmap(0,aligned_size,PROT_READ|PROT_WRITE,MAP_SHARED,devfd,offset*getpagesize());
}
int npheap_lock(int devfd, __u64 offset)
{
     struct npheap_cmd cmd;
     cmd.offset = offset*getpagesize();     
     return ioctl(devfd, NPHEAP_IOCTL_LOCK, &cmd);
}

int npheap_unlock(int devfd, __u64 offset)
{
     struct npheap_cmd cmd;
     cmd.offset = offset*getpagesize();     
     return ioctl(devfd, NPHEAP_IOCTL_UNLOCK, &cmd);
}

int npheap_delete(int devfd, __u64 offset)
{
     struct npheap_cmd cmd;
     cmd.offset = offset*getpagesize();
     return ioctl(devfd, NPHEAP_IOCTL_DELETE, &cmd);
}

long npheap_getsize(int devfd, __u64 offset)
{
     struct npheap_cmd cmd;
     cmd.offset = offset*getpagesize();
     return ioctl(devfd, NPHEAP_IOCTL_GETSIZE, &cmd);
}
