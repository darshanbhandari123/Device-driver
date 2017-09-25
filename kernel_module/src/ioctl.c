// Project 1: Laxmikant Kishor Mokadam, lmokada; Ananthram Eklaspuram Lakshmanasamy, aeklasp; Darshan Balakrishna Bhandari, dbhanda; 
//////////////////////////////////////////////////////////////////////
//                             North Carolina State University
//
//
//
//                             Copyright 2016
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng
//
//   Description:
//     Skeleton of NPHeap Pseudo Device
//
////////////////////////////////////////////////////////////////////////
// References: http://www.makelinux.net/ldd3/chp-11-sect-5, https://isis.poly.edu/kulesh/stuff/src/klist/


#include "npheap.h"
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>
#include <linux/list.h>
#include <linux/mutex.h>


// using the same np_list struct as in the core.c
struct np_list
{
    long unsigned int objid;
    int sem;
    long unsigned int address;
    long unsigned int size;
    struct list_head list;
    void * data;
} ;


// getting the list head from core.c
extern struct list_head np_addr_list;



static DEFINE_MUTEX(cache_lock);
// If exist, return the data.

long npheap_lock(struct npheap_cmd __user *user_cmd) {
    // locking the queue
    mutex_lock(&cache_lock);
    return 0;
}

long npheap_unlock(struct npheap_cmd __user *user_cmd) {
    // unlocking the queue
    mutex_unlock(&cache_lock);
    return 0;
}

long npheap_getsize(struct npheap_cmd __user *user_cmd) {
    struct np_list *tmp;
    struct list_head *pos;
    // allocating the kernel memory to save the object
    struct npheap_cmd *local = kmalloc(sizeof (struct npheap_cmd), GFP_KERNEL);
    
    // copying the object from user space to kernel space  
    copy_from_user(local, user_cmd, sizeof (struct npheap_cmd));
    list_for_each(pos, &np_addr_list)
    {
        // getting the node from the list 
        tmp = list_entry(pos, struct np_list, list);
        // checking for object id
        if (tmp->objid == local->offset/PAGE_SIZE)
        {
            // if found the objectid, return , else -1
            return tmp->size;
        }
    }

    return 0;

}

long npheap_delete(struct npheap_cmd __user *user_cmd) {

     struct np_list *tmp;
    struct list_head *pos, *q;
    struct npheap_cmd *local = kmalloc(sizeof (struct npheap_cmd), GFP_KERNEL);
    
    // coping the user data into kernel and accessing the object id rom it. 
    copy_from_user(local, user_cmd, sizeof (struct npheap_cmd));
    list_for_each_safe(pos, q,&np_addr_list)
    {
        tmp = list_entry(pos, struct np_list, list);
        // check for object id
        if (tmp->objid == local->offset/PAGE_SIZE)
        {
            // decrementing the refrence counter
            tmp->sem--;
            // if reference counter reaches zero, it will delete the node
            if(tmp->sem == 0){
		kfree(tmp->data);
                list_del(pos);
                kfree(tmp);
            }          
        }
    }

    return 0;
}

long npheap_ioctl(struct file *filp, unsigned int cmd,
        unsigned long arg) {
    switch (cmd) {
        case NPHEAP_IOCTL_LOCK:
            return npheap_lock((void __user *) arg);
        case NPHEAP_IOCTL_UNLOCK:
            return npheap_unlock((void __user *) arg);
        case NPHEAP_IOCTL_GETSIZE:
            return npheap_getsize((void __user *) arg);
        case NPHEAP_IOCTL_DELETE:
            return npheap_delete((void __user *) arg);
        default:
            return -ENOTTY;
    }
}

