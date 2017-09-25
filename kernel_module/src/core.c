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
//We have used the Kernel implementation of LinkedList (to store the mapping) by including list.h

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
#include <linux/mutex.h>
#include <linux/list.h>

extern struct miscdevice npheap_dev;


// Declaring the struct to store relevent data of the object
struct np_list
{
    long unsigned int objid;    // Stores object id
    int sem;                    // Stores the counter, for each new refrence, it will add by one. 
                                // And after each derefrence, it will decement. Delete the node after refrence reached zero.
    long unsigned int address;  // storing the pfn of the object
    long unsigned int size;     // storing the size of the object
    struct list_head list;      
    void * data;                // storing pointer to the object data in kernel space.
} ;

struct list_head np_addr_list ;
LIST_HEAD(np_addr_list);    // Initializing the list 

struct np_list *tmp;   

struct list_head *pos;  //  store the current node during traversing the linked list

int npheap_mmap(struct file *filp, struct vm_area_struct *vma)
{
    list_for_each(pos, &np_addr_list) // traverse the linked list 
    {
        tmp = list_entry(pos, struct np_list, list); // receiving the list node 
        
        if (tmp != NULL && tmp->objid == vma->vm_pgoff) // object IDs match 
        {
            // remaping the virtual address to physical address
            if (remap_pfn_range(vma,
                                vma->vm_start,
                                tmp->address,
                                vma->vm_end - vma->vm_start,
                                vma->vm_page_prot))
                return -EAGAIN;
            // For each succsseful reference, increaseing the sem (refrence counter ) by 1
            tmp->sem = tmp->sem + 1;
            return 0;
        }
    }
    // if object not in list(not already mapped), create memory and store object in the list
    void *alloc_addr = kmalloc(((vma->vm_end - vma->vm_start)), GFP_KERNEL);

    if (alloc_addr == NULL)
    {
        //kmalloc fails to allocate memomry 
        return -1; 
    }

    else
    {
        //when kmalloc succesfully allocates memory 
        tmp = (struct np_list *)kmalloc(sizeof(struct np_list),GFP_KERNEL);
        tmp->objid = vma->vm_pgoff;
        tmp->address = virt_to_phys((void *)((long unsigned int)alloc_addr)) >> PAGE_SHIFT;
        tmp->sem = 1;
        tmp->size=  (unsigned long int) vma->vm_end - vma->vm_start ; 
        tmp->data = alloc_addr;
        
        //map virtual address to phy addrress 
        if (remap_pfn_range(vma,
                    vma->vm_start,
                    virt_to_phys((void*) ((long unsigned int) alloc_addr)) >> PAGE_SHIFT,
                    vma->vm_end - vma->vm_start,
                    vma->vm_page_prot)) return -EAGAIN;
        // adding f=the object in list
        list_add(&(tmp->list), &(np_addr_list));
        return 0;

    }
}

int npheap_init(void)
{
    int ret;
    if ((ret = misc_register(&npheap_dev)))
        printk(KERN_ERR "Unable to register \"npheap\" misc device\n");
    else
        printk(KERN_ERR "\"npheap\" misc device installed\n");
    return ret;
}

void npheap_exit(void)
{
    misc_deregister(&npheap_dev);
}

