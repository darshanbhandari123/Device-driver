#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <npheap.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i=0,number_of_threads = 1, number_of_objects=1024, max_size_of_objects=8192; 
    int tid;
    long size;
    __u64 object_id;
    __u64 current_time;
    char *data,op,*mapped_data;
    char **obj;
    int devfd;
    int error = 0;
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s number_of_objects max_size_of_objects\n",argv[0]);
        exit(1);
    }
    number_of_objects = atoi(argv[1]);
    if(argc > 2)
        max_size_of_objects = atoi(argv[2]);
    data = (char *)malloc(max_size_of_objects*sizeof(char));
    obj = (char **)malloc(number_of_objects*sizeof(char *));
    for(i = 0; i < number_of_objects; i++)
    {
        obj[i] = (char *)calloc(max_size_of_objects, sizeof(char));
    }
    // Replay the log
    // Validate
    while(scanf("%c %d %llu %llu %ld %s",&op, &tid, &current_time, &object_id, &size, &data[0])!=EOF)
    {
        if(op == 'S')
        {
            strcpy(obj[(int)object_id],data);
            memset(data,0,max_size_of_objects);
        } else if (op == 'G') {
            if (strcmp(obj[(int)object_id], data)) {   
                fprintf(stderr, "%d: Key %d has a wrong value %s v.s. %s\n",tid,(int)object_id,data,obj[(int)object_id]);
                error++; 
            }
        }
        else if (op == 'D') {
            memset(obj[(int)object_id],0,max_size_of_objects);
        }
        if (error > 5) {
            break;
        }
    }
    devfd = open("/dev/npheap",O_RDWR);
    if(devfd < 0)
    {
        fprintf(stderr, "Device open failed");
        exit(1);
    }
    for(i = 0; i < number_of_objects; i++)
    {
        size = npheap_getsize(devfd,i);
        if(size!=0)
        {
            mapped_data = (char *)npheap_alloc(devfd,i,npheap_getsize(devfd,i));
            if(strcmp(mapped_data,obj[i])!=0)
            {
                 fprintf(stderr, "Object %d has a wrong value %s v.s. %s\n",i,mapped_data,obj[i]);
                 error++;
            }
        }
        else
        {
            mapped_data = NULL;
            if(strlen(obj[i])!=0)
            {
                 fprintf(stderr, "Object %d should have a value %s\n",i,obj[i]);
                 error++;
            }
        }
    }
    if(error == 0)
        fprintf(stderr,"Pass\n");
    close(devfd);
    return 0;
}

