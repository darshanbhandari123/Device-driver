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
//     Skeleton of KeyValue Pseudo Device
//
////////////////////////////////////////////////////////////////////////


#ifndef _UAPI_KEYVALUE_H
#define _UAPI_KEYVALUE_H

#include <linux/types.h>


struct npheap_cmd {
    __u64 op;	// 0 for lock, 1 for unlock
    __u64 offset;
    __u64 size;
    void *data;
};

#define NPHEAP_IOCTL_LOCK  _IOWR('N', 0x43, struct npheap_cmd)
#define NPHEAP_IOCTL_UNLOCK  _IOWR('N', 0x44, struct npheap_cmd)
#define NPHEAP_IOCTL_DELETE  _IOWR('N', 0x45, struct npheap_cmd)
#define NPHEAP_IOCTL_GETSIZE  _IOWR('N', 0x46, struct npheap_cmd)

#endif
