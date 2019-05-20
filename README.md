# os-simulation
This is a simulation of an operating system.<br/>
to run
```
$make
$./os
```
When it start you will be prompeted to enter the amount of ram you, page table size, and disk. <br/>

## Commands available <br/>
A       ‘A’ input means that a new process has been created. When a new process arrives, your program should create its PCB and place the process in the end of the ready-queue. Initially, no memory is allocated for the new process.

Q       The currently running process has spent a time quantum using the CPU. Such process should be preempted to the end of the ready-queue.

 

fork       The process forks a child. The child is placed in the end of the ready-queue.

 

exit         The process that is currently using the CPU terminates. Make sure you release the memory used by this process immediately. To avoid the appearance of the orphans, the system implements the cascading termination (see comments below). If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the end of the ready-queue). If its parent hasn’t called wait yet, the process turns into zombie.

 

wait        The process wants to pause and wait for any of its child processes to terminate. Once the wait is over, the process goes to the end of the ready-queue. If the zombie-child already exists, the process proceeds right away (keeps using the CPU) and the zombie-child disappears. If more then one zombie-child existed, the system uses one of them (any!) to immediately restart the parent and other zombies keep waiting for the next wait from the parent.

 

d number file_name       The process that currently uses the CPU requests the hard disk #number. It wants to read or write file file_name.

 

D number   The hard disk #number has finished the work for one process.

 

m address   The process that is currently using the CPU requests a memory operation for the logical address.

 

S r     Shows what process is currently using the CPU and what processes are waiting in the ready-queue.

 

S i      Shows what processes are currently using the hard disks and what processes are waiting to use them. For each busy hard disk show the process that uses it and show its I/O-queue. Make sure to display the filenames (from the d command) for each process. The enumeration of hard disks starts from 0.

 

S m   Shows the state of memory. For each used frame display the process number that occupies it and the page number stored in it. The enumeration of pages and frames starts from 0.
