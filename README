Overview

Modern operating systems promote virtualization of their underlying machines and isolation among processes. As a result, communicating among processes becomes an issue. You may need to use message passing interfaces, sockets or pipes that works only when processes are running concurrently. Otherwise, you have to use files that rely on file system abstraction and relatively slow secondary devices. 

To address the above issue, we will explore and examine an idea, NP-Heap, non-persistent heap. NP-Heap leverages the Mach-style memory space layout that all modern operating systems uses -- all processes reserve a shared, but user mode inaccessible kernel space memory. NP-Heap provides an interface that exposes this kernel mode only memory to user process so that any process can read, write these protected but shared memory locations. Since the operating system do not clear states that the kernel code maintains, NP-Heap can keep data for a later spawned process even if the source process vanished. In addition, NP-Heap simply rely on the main memory and memory semantics to manage data for processes. In this way, NP-Heap does not suffer from the file system overhead that conventional mechanisms cannot bypass.

As operating systems only allow kernel code to manage and access kernel space memory by default, implementing NP-Heap would require additional efforts in any operating system kernel. Fortunately, most modern operating systems support "loadable kernel modules". In this way, the system can boot with a simpler, smaller kernel and then load these modules into kernel space when necessary. In this project, we will implement NP-Heap as a loadable kernel module as well as set of library functions that create a pseudo device in the system and provide an interface for applications. By interacting with this device, processes can map user-space virtual memory locations into kernel space memory locations. To support concurrent execution, NP-Heap will also provide functions and commands for locking/unlocking.

You are strongly encouraged to work in a group of 2 or 3. Groups do the same project as individuals. All members will receive the same grade. Note that working in groups may or may not make the project easier, depending on how the group interactions work out. If collaboration issues arise, contact your instructor as soon as possible: flexibility in dealing with such issues decreases as the deadline approaches. 

In this project, you will be given the prototype of the kernel module with a interface.c file that only contains empty functions. We also provide a user-space library that allows an application to interact with this kernel module through ioctl/mmap interfaces as well as a sample benchmark application that you may extend to test if your kernel module functions correctly.

Objective

* Learning UNIX/Linux kernel programming as well as the constraints
* Learning UNIX/Linux system memory management
* Learning UNIX/Linux kernel modules
* Learning multiprocess programming

How to start 

To begin, you need to first form a group and setup the environment for developing your project. You should set up a machine or a VMWare virtual machine (CS students should have free license for that) with clean Ubuntu 16.04 installation. You also may use the VCL service maintained by NCSU through https://vcl.ncsu.edu/ . You may reserve one virtual machine and connect to this machine remotely by selecting reservations. We will use the "Ubuntu 16.04 Base" to test your kernel module. However, the VCL service will reset once your reservation timeout. 

Then, you may fetch the code from https://github.ncsu.edu/htseng3/CSC501_NPHeap.git.  After fetching the code, you will find three directories and a test script. These directories are:

1. kernel_module -- the directory where we have the kernel module code.

2. library -- the directory of the user-space library code.

3. benchmark -- the directory with a sample program using this kernel and a validator to examine the result.

You may now go to the kernel_module directory and type "make" to compile the kernel module and then "sudo make install" to install headers and the module in the right place. You should be able to find an npheap.ko if your compilation success and this is the binary of the npheap kernel module. However, this kernel module isn't in your kernel yet. To get this kernel module loaded into the system kernel, try "sudo insmod npheap.ko". Upon success, you should find an "npheap" device file under /dev directory in your linux system. By default, this device may not be available for non-root users. Therefore, you need to use "sudo chmod 777 /dev/npheap" command to make it accessible by anyone and any process in the system. If you don't want this device to be available in the system anymore, you can use "sudo rmmod npheap" to remove this device.

Now, you can navigate to the library path and again use "make" to generate this dynamic link library. You need to then use "sudo make install" to make this library publicly available for the system. You should read the code and figure out how this library interacts with the kernel module. 

Finally, you can now go to the benchmark directory to get the benchmark and validate programs compiled. You may use the test.sh to have a trial run of the whole framework.

No matter you're using VMWare, real machine, or VCL, you should always use https://github.ncsu.edu to control/maintain/backup your work.
