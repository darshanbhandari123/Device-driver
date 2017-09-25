#ifdef __cplusplus
extern "C" {
#endif
#include <linux/types.h>
void *npheap_alloc(int devfd, __u64 offset, __u64 size);
int npheap_lock(int devfd, __u64 offset);
int npheap_unlock(int devfd, __u64 offset);
int npheap_delete(int devfd, __u64 offset);
long npheap_getsize(int devfd, __u64 offset);
#ifdef __cplusplus
}
#endif