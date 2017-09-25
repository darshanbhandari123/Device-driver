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

extern long npheap_lock(struct npheap_cmd __user *user_cmd);
extern long npheap_unlock(struct npheap_cmd __user *user_cmd);
extern long npheap_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
extern int npheap_mmap(struct file *filp, struct vm_area_struct *vma);
extern int npheap_init(void);
extern void npheap_exit(void);

static const struct file_operations npheap_fops = {
    .owner                = THIS_MODULE,
    .unlocked_ioctl       = npheap_ioctl,
    .mmap                 = npheap_mmap,
};

struct miscdevice npheap_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "npheap",
    .fops = &npheap_fops,
};

static int __init npheap_module_init(void)
{
    return npheap_init();
}

static void __exit npheap_module_exit(void)
{
    npheap_exit();
}


MODULE_AUTHOR("Hung-Wei Tseng <htseng3@ncsu.edu>");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
module_init(npheap_module_init);
module_exit(npheap_module_exit);
